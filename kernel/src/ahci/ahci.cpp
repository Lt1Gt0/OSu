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
        uint32 portsImplemented = this->mABAR->portsImplemented;
        for (int i = 0; i < 32; i++) {
            if (portsImplemented & (1 << i)) {
                PortType portType = CheckPortType(&this->mABAR->ports[i]);
                if (portType == PortType::SATA || portType == PortType::SATAPI) {
                    this->mPorts[this->mPortCount] = new Port();
                    this->mPorts[this->mPortCount]->mPortType = portType;
                    this->mPorts[this->mPortCount]->mHBAPort = &this->mABAR->ports[i];
                    this->mPorts[this->mPortCount]->mPortNumber = this->mPortCount;
                    this->mPortCount++;
                }
            }
        }
    }

    void Port::Configure()
    {
        StopCMD();

        void* newBase = GlobalAllocator.RequestPage();
        this->mHBAPort->commandListBase = (uint32)(uint64)newBase;
        this->mHBAPort->commandListBaseUpper = (uint32)((uint64)newBase >> 32);
        memset((void*)(&this->mHBAPort->commandListBase), 0, 1024);

        void* fisBase = GlobalAllocator.RequestPage();
        this->mHBAPort->fisBaseAddress = (uint32)(uint64)fisBase;
        this->mHBAPort->fisBaseAddressUpper = (uint32)((uint64)fisBase >> 32);
        memset(fisBase, 0, 256);

        HBACommandHeader* cmdHeader = (HBACommandHeader*)((uint64)this->mHBAPort->commandListBase + ((uint64)this->mHBAPort->commandListBaseUpper << 32));

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
        while(this->mHBAPort->cmdSts & HBA_PxCMD_CR);

        this->mHBAPort->cmdSts |= HBA_PxCMD_FRE;
        this->mHBAPort->cmdSts |= HBA_PxCMD_ST;
    }

    void Port::StopCMD()
    {
        this->mHBAPort->cmdSts &= ~HBA_PxCMD_ST;
        this->mHBAPort->cmdSts &= ~HBA_PxCMD_FRE;

        while (true) {
            if (this->mHBAPort->cmdSts & HBA_PxCMD_FR
             || this->mHBAPort->cmdSts & HBA_PxCMD_CR)
                continue;

            break;
        }
    }

    bool Port::Read(uint64 sector, uint32 sectorCount, void* buffer)
    {
        uint64 spin = 0;
        while ((this->mHBAPort->taskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
            spin++;
        }

        if (spin == 1000000)
            return false;
    
        uint32 sectorL = (uint32)sector;
        uint32 sectorH = (uint32)(sector >> 32);

        // Clear pending interrupt bits
        this->mHBAPort->interruptStatus  = (uint32) -1;

        HBACommandHeader* cmdHeader = (HBACommandHeader*)&this->mHBAPort->commandListBase;

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

        this->mHBAPort->commandIssue = 1;

        while (true) {
            if (this->mHBAPort->commandIssue == 0)
                break;
            
            if (this->mHBAPort->interruptStatus & HBA_PxIS_TFES)
                return false;
        }

        return true;
    }

    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress)
    {
        this->mPCIBaseAddress = pciBaseAddress;
        kprintf("AHCI Driver instance initialized\n");

        this->mABAR = (HBAMemory*)(&((PCI::PCIHeader0*)pciBaseAddress)->BAR5);

        PageTableManager::MapMemory(this->mABAR, this->mABAR);
        ProbePorts();

        // for (int i = 0; i < portCount; i++) {
        //     Port* port = ports[i];

        //     port->Configure();

        //     port->buffer = (uint8*)GlobalAllocator.RequestPage();
        //     memset(port->buffer, 0, 0x1000);

        //     port->Read(0, 4, port->buffer);

        //     for (int t = 0; t < 1024; t++) {
        //         GlobalRenderer.PutChar(port->buffer[t]);
        //     }

        //     GlobalRenderer.Next();
        // }
    }

    AHCIDriver::~AHCIDriver()
    {

    }
}
