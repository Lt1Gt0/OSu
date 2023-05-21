#include <ahci/ahci.h>

#include <renderer.h>
#include <paging/pagetableManager.h>
#include <memory/heap.h>
#include <paging/pageframeAllocator.h>
#include <memory.h>
#include <print.h>

namespace AHCI 
{
    PortType CheckPortType(HBAPort* port)
    {
        uint32 sataStatus = port->sataStatus;

        uint8 interfacePowerManagment = (sataStatus >> 8) & 0b111;
        uint8 deviceDetection = sataStatus & 0b111;

        if (deviceDetection != HBA_PORT_DEB_PRESENT)
            return PortType::None;

        if (interfacePowerManagment != HBA_PORT_IPM_ACTIVE)
            return PortType::None;

        switch (port->signature) {
			case SATA_SIG_ATAPI:
				return PortType::SATAPI;
			case SATA_SIG_ATA:
				return PortType::SATA;
			case SATA_SIG_PM:
				return PortType::PM;
			case SATA_SIG_SEMB:
				return PortType::SEMB;
			default:
				return PortType::None;
        }
    }

    void AHCIDriver::ProbePorts()
    {
        uint32 portsImplemented = mABAR->portsImplemented;
        for (int i = 0; i < 32; i++) {
            if (portsImplemented & (1 << i)) {
                PortType portType = CheckPortType(&mABAR->ports[i]);
                if (portType == PortType::SATA || portType == PortType::SATAPI) {
                    mPorts[mPortCount] = new Port();
                    mPorts[mPortCount]->mPortType = portType;
                    mPorts[mPortCount]->mHBAPort = &mABAR->ports[i];
                    mPorts[mPortCount]->mPortNumber = mPortCount;
                    mPortCount++;
                }
            }
        }
    }

    void Port::Configure()
    {
        StopCMD();

        void* newBase = GlobalAllocator.RequestPage();
        mHBAPort->commandListBase = (uint32)(uint64)newBase;
        mHBAPort->commandListBaseUpper = (uint32)((uint64)newBase >> 32);
        memset((void*)(&mHBAPort->commandListBase), 0, 1024);

        void* fisBase = GlobalAllocator.RequestPage();
        mHBAPort->fisBaseAddress = (uint32)(uint64)fisBase;
        mHBAPort->fisBaseAddressUpper = (uint32)((uint64)fisBase >> 32);
        memset(fisBase, 0, 256);

        HBACommandHeader* cmdHeader = (HBACommandHeader*)((uint64)mHBAPort->commandListBase + ((uint64)mHBAPort->commandListBaseUpper << 32));

        for (int i = 0; i < 32; i++) {
            cmdHeader[i].prdtLength = 8;
            void* cmdTableAddress = GlobalAllocator.RequestPage();
            uint64 address = (uint64)cmdTableAddress + (i << 8);
            cmdHeader[i].commandTableBaseAddress = (uint32)(uint64)address;
            cmdHeader[i].commandTableBaseAddressUpper = (uint32)((uint64)address >> 32);
            memset(cmdTableAddress, 0, 256);
        }

        StartCMD();
    }

    void Port::StartCMD()
    {
        while(mHBAPort->cmdSts & HBA_PxCMD_CR);

        mHBAPort->cmdSts |= HBA_PxCMD_FRE;
        mHBAPort->cmdSts |= HBA_PxCMD_ST;
    }

    void Port::StopCMD()
    {
        mHBAPort->cmdSts &= ~HBA_PxCMD_ST;
        mHBAPort->cmdSts &= ~HBA_PxCMD_FRE;

        while (true) {
            if (mHBAPort->cmdSts & HBA_PxCMD_FR
             || mHBAPort->cmdSts & HBA_PxCMD_CR)
                continue;

            break;
        }
    }

    bool Port::Read(uint64 sector, uint32 sectorCount, void* buffer)
    {
        uint64 spin = 0;
        while ((mHBAPort->taskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
            spin++;
        }

        if (spin == 1000000)
            return false;
    
        uint32 sectorL = (uint32)sector;
        uint32 sectorH = (uint32)(sector >> 32);

        // Clear pending interrupt bits
        mHBAPort->interruptStatus  = (uint32) -1;

        HBACommandHeader* cmdHeader = (HBACommandHeader*)&mHBAPort->commandListBase;

        // Command FIS Size
        cmdHeader->commandFISLength = sizeof(FIS_REG_H2D) / sizeof(uint32);
        cmdHeader->write = 0;
        cmdHeader->prdtLength = 1;

        HBACommandTable* commandTable = (HBACommandTable*)(&cmdHeader->commandTableBaseAddress);
        memset(commandTable, 0, sizeof(HBACommandTable) + (cmdHeader->prdtLength - 1) * sizeof(HBAPRDTEntry));

        commandTable->prdtEntry[0].dataBaseAddress = (uint32)(uint64)buffer;
        commandTable->prdtEntry[0].dataBaseAddressUpper = (uint32)((uint64)buffer >> 32);
        commandTable->prdtEntry[0].byteCount = (sectorCount << 9) - 1; // 512 bytes per sector
        commandTable->prdtEntry[0].interruptOnCompletion = 1;

        FIS_REG_H2D* cmdFIS = (FIS_REG_H2D*)(&commandTable->commandFIS);

		cmdFIS->fisType = FIS_TYPE::REG_H2D;
        cmdFIS->commandControl = 1;
        cmdFIS->command = ATA_CMD_READ_DMA_EX;
        cmdFIS->lba0 = (uint8)sectorL;
        cmdFIS->lba1 = (uint8)(sectorL >> 8);
        cmdFIS->lba2 = (uint8)(sectorL >> 16);
        cmdFIS->lba3 = (uint8)sectorH;
        cmdFIS->lba4 = (uint8)(sectorH >> 8);
        cmdFIS->lba5 = (uint8)(sectorH >> 16);

        cmdFIS->deviceRegister = 1 << 6; // LBA Mode

        cmdFIS->countLow = sectorCount & 0xFF;
        cmdFIS->countHigh = (sectorCount >> 8) & 0xFF;

        mHBAPort->commandIssue = 1;

        while (true) {
            if (mHBAPort->commandIssue == 0)
                break;
            
            if (mHBAPort->interruptStatus & HBA_PxIS_TFES)
                return false;
        }

        return true;
    }

    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress)
    {
        mPCIBaseAddress = pciBaseAddress;
        kprintf("AHCI Driver instance initialized\n");

        mABAR = (HBAMemory*)(&((PCI::PCIHeader0*)pciBaseAddress)->BAR5);

        PageTableManager::MapMemory(mABAR, mABAR);
        ProbePorts();

        for (int i = 0; i < mPortCount; i++) {
            Port* port = mPorts[i];

            port->Configure();

            port->mBuffer = (uint8*)GlobalAllocator.RequestPage();
            memset(port->mBuffer, 0, PAGE_SIZE);

            port->Read(0, 4, port->mBuffer);

            for (int t = 0; t < 1024; t++) {
                GlobalRenderer.PutChar(port->mBuffer[t]);
            }

            GlobalRenderer.Next();
        }
    }

    AHCIDriver::~AHCIDriver()
    {

    }
}
