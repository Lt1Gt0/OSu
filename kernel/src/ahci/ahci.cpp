#include <ahci/ahci.h>

namespace AHCI 
{
    PortType CheckPortType(HBAPort* port)
    {
        uint32_t sataStatus = port->sataStatus;

        uint8_t interfacePowerManagment = (sataStatus >> 8) & 0b111;
        uint8_t deviceDetection = sataStatus & 0b111;

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
        uint32_t portsImplemented = ABAR->portsImplemented;
        for (int i = 0; i < 32; i++) {
            if (portsImplemented & (1 << i)) {
                PortType portType = CheckPortType(&ABAR->ports[i]);

                if (portType == PortType::SATA) {
                    GlobalRenderer->PrintLine("(Sata Drive)");
                } else if (portType == PortType::SATAPI) {
                    GlobalRenderer->PrintLine("(SATAPI Drive)");
                } else {
                    GlobalRenderer->PrintLine("(Not Interested)");
                }
            }
        }
    }

    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress)
    {
        this->PCIBaseAddress = pciBaseAddress;
        GlobalRenderer->PrintLine("AHCI Driver instance initialized", 0xFF00FF00);

        ABAR = (HBAMemory*)((PCI::PCIHeader0*)pciBaseAddress)->BAR5;

        g_PageTableManager.MapMemory(ABAR, ABAR);
        ProbePorts();
    }

    AHCIDriver::~AHCIDriver()
    {

    }
}