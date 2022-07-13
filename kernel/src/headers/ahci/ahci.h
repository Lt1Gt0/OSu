#pragma once
#ifndef _AHCI_H
#define _AHCI_H

#include <types.h>
#include <pci.h>

namespace AHCI
{
    #define HBA_PORT_IPM_ACTIVE     0x1
    #define HBA_PORT_DEB_PRESENT    0x3

    #define HBA_PxCMD_ST            0x0001
    #define HBA_PxCMD_FRE           0x0010
    #define HBA_PxCMD_FR            0x4000
    #define HBA_PxCMD_CR            0x8000

    #define SATA_SIG_ATA            0x00000101
    #define SATA_SIG_PM             0x96690101
    #define SATA_SIG_SEMB           0xC33C0101
    #define SATA_SIG_ATAPI          0xEB140101

    #define HBA_PxIS_TFES           (1 << 30)
    #define ATA_DEV_DRQ             0x08
    #define ATA_CMD_READ_DMA_EX     0x25
    #define ATA_DEV_BUSY            0x80

    enum PortType {
        None,
        SATA,
        SEMB,
        PM,
        SATAPI,
    };

    enum FIS_TYPE : uint8 {
        FIS_TYPE_REG_H2D    = 0x27, // Register FIS - host to device
        FIS_TYPE_REG_D2H    = 0x34, // Register FIS - device to host
        FIS_TYPE_DMA_ACT    = 0x39, // DMA activate FIS - device to host
        FIS_TYPE_DMA_SETUP  = 0x41, // DMA setup FIS - bidirectional
        FIS_TYPE_DATA       = 0x46, // Data FIS - bidirectional
        FIS_TYPE_BIST       = 0x58, // BIST activate FIS - bidirectional
        FIS_TYPE_PIO_SETUP  = 0x5F, // PIO setup FIS - device to host
        FIS_TYPE_DEV_BITS   = 0xA1, // Set device bits FIS - device to host
    };

    struct HBAPort {
        uint32 commandListBase;
        uint32 commandListBaseUpper;
        uint32 fisBaseAddress;
        uint32 fisBaseAddressUpper;
        uint32 interruptStatus;
        uint32 interruptEnable;
        uint32 cmdSts;
        uint32 rsv0;
        uint32 taskFileData;
        uint32 signature;
        uint32 sataStatus;
        uint32 sataControl;
        uint32 sataError;
        uint32 sataActive;
        uint32 commandIssue;
        uint32 sataNotification;
        uint32 fisSwitchControl;
        uint32 rsv1[11];
        uint32 ports[4];
    };

    struct HBAMemory {
        uint32  hostCapabilitiy;
        uint32  globalHostControl;
        uint32  interruptStatus;
        uint32  portsImplemented;
        uint32  version;
        uint32  cccControl;
        uint32  cccPorts;
        uint32  enclosureManagmentLocation;
        uint32  enclosureManagmentControl;
        uint32  hostCapabilitiesExtended;
        uint32  biosHandoffCtrlSts;
        uint8   rsv0[0x74];
        uint8   vendor[0x60];
        HBAPort ports[1];
    };

    struct HBACommandHeader {
        uint8   commandFISLength    : 5;
        uint8   atapi               : 1;
        uint8   write               : 1;
        uint8   prefetchable        : 1;

        uint8   reset               : 1;
        uint8   bist                : 1;
        uint8   clearBusy           : 1;
        uint8   rsv0                : 1;
        uint8   portMultiplier      : 4;
        
        uint16  prdtLength;
        uint32  prdbCount;
        uint32  commandTableBaseAddress;
        uint32  commandTableBaseAddressUpper;
        uint32  rsv1[4];
    };

    struct HBAPRDTEntry {
        uint32 dataBaseAddress;
        uint32 dataBaseAddressUpper;
        uint32 rsv0;

        uint32 byteCount              : 22;
        uint32 rsv1                   : 9;
        uint32 interruptOnCompletion  : 1;
    };

    struct HBACommandTable {
        uint8           commandFIS[64];
        uint8           atapiCommand[16];
        uint8           rsv[48];
        HBAPRDTEntry    prdtEntry[];
    };

    struct FIS_REG_H2D {
        uint8 fisType;
        
        uint8 portMultiplier  : 4;
        uint8 rsv0            : 3;
        uint8 commandControl  : 1; // 1: Command, 0: Control
    
        uint8 command;
        uint8 featureLow;
    
        uint8 lba0;
        uint8 lba1;
        uint8 lba2;
        uint8 deviceRegister;
    
        uint8 lba3;
        uint8 lba4;
        uint8 lba5;
        uint8 featureHigh;
    
        uint8 countLow;
        uint8 countHigh;
        uint8 isoCommandCompletion;
        uint8 control;
    
        uint8 rsv1[4];
    };

    class Port
    {
        public:
            HBAPort*    hbaPort;
            PortType    portType;
            uint8*      buffer;
            uint8       portNumber;

            void        Configure();
            void        StartCMD();
            void        StopCMD();
            bool        Read(uint64 sector, uint32 sectorCount, void* buffer);
    };

    class AHCIDriver
    {
        public:
            AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress);
            ~AHCIDriver();

            PCI::PCIDeviceHeader* PCIBaseAddress;
            HBAMemory* ABAR;

            void ProbePorts();

            Port* ports[32];
            uint8 portCount;
    };
}

#endif // _ACHI_H