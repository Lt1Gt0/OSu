#pragma once
#include <stdint.h>
#include <pci.h>
#include <BasicRenderer.h>
#include <paging/PageTableManager.h>

namespace AHCI
{
    #define HBA_PORT_IPM_ACTIVE 0x1

    #define HBA_PORT_DEB_PRESENT 0x3

    #define SATA_SIG_ATAPI 0xEB140101
    #define SATA_SIG_ATA 0x00000101
    #define SATA_SIG_SEMB 0xC33C0101
    #define SATA_SIG_PM 0x96690101

    enum PortType {
        None,
        SATA,
        SEMB,
        PM,
        SATAPI,
    };

    struct HBAPort {
        uint32_t commandListBase;
        uint32_t commandListBaseUpper;
        uint32_t fisBaseAddress;
        uint32_t fisBaseAddressUpper;
        uint32_t interruptStatus;
        uint32_t interruptEnable;
        uint32_t cmdSts;
        uint32_t rsv0;
        uint32_t taskFileData;
        uint32_t signature;
        uint32_t sataStatus;
        uint32_t sataControl;
        uint32_t sataError;
        uint32_t sataActive;
        uint32_t commandIssue;
        uint32_t sataNotification;
        uint32_t fisSwitchControl;
        uint32_t rsv1[11];
        uint32_t ports[4];
    };

    struct HBAMemory {
        uint32_t hostCapabilitiy;
        uint32_t globalHostControl;
        uint32_t interruptStatus;
        uint32_t portsImplemented;
        uint32_t version;
        uint32_t cccControl;
        uint32_t cccPorts;
        uint32_t enclosureManagmentLocation;
        uint32_t enclosureManagmentControl;
        uint32_t hostCapabilitiesExtended;
        uint32_t biosHandoffCtrlSts;
        uint8_t  rsv0[0x74];
        uint8_t  vendor[0x60];
        HBAPort  ports[1];
    };

    class AHCIDriver
    {
        public:
            AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress);
            ~AHCIDriver();

            PCI::PCIDeviceHeader* PCIBaseAddress;
            HBAMemory* ABAR;

            void ProbePorts();
    };
}