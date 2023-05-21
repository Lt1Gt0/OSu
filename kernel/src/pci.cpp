#include <ahci/ahci.h>
#include <pci.h>
#include <print.h>

namespace PCI 
{
    void EnumerateFunction(uint64 deviceAddress, uint64 function) 
    {
        uint64 offset = function << 12;
        uint64 functionAddress = deviceAddress + offset;
        PageTableManager::MapMemory((void*)functionAddress, (void*)functionAddress);
        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader* )functionAddress;

        if (pciDeviceHeader->DeviceID == 0 || pciDeviceHeader->DeviceID == 0xFFFF)
            return;

        kprintf(
            "%s0 | %s1 | %s2 | %s3 | %s4 | %s5\n",
            (nint)GetVendorName(pciDeviceHeader->VendorID),
            (nint)GetDeviceName(pciDeviceHeader->VendorID, pciDeviceHeader->DeviceID),
            (nint)DeviceClasses[pciDeviceHeader->Class],
            (nint)GetSubclassName(pciDeviceHeader->Class, pciDeviceHeader->Subclass),
            (nint)GetProgIFName(pciDeviceHeader->Class, pciDeviceHeader->Subclass,
            pciDeviceHeader->ProgIF));

        switch (pciDeviceHeader->Class) {
            case 0x01: // Mass Storage Controller
            switch (pciDeviceHeader->Subclass) {
                case 0x06: // Serial ATA
                    switch (pciDeviceHeader->ProgIF) 
                    {
                        case 0x01: // AHCI 1.0 Device
                            new AHCI::AHCIDriver(pciDeviceHeader);
                    }
            }
        }
    }

    void EnumerateDevice(uint64 busAddress, uint64 device) 
    {
        uint64 offset = device << 15;
        uint64 deviceAddress = busAddress + offset;
        PageTableManager::MapMemory((void*)deviceAddress, (void*)deviceAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)deviceAddress;

        if (pciDeviceHeader->DeviceID == 0 || pciDeviceHeader->DeviceID == 0xFFFF)
            return;

        for (uint64 function = 0; function < 8; function++) {
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64 baseAddress, uint64 bus) 
    {
        uint64 offset = bus << 20;
        uint64 busAddress = baseAddress + offset;
        PageTableManager::MapMemory((void*)busAddress, (void*)busAddress);
        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)busAddress;

        if (pciDeviceHeader->DeviceID == 0 || pciDeviceHeader->DeviceID == 0xFFFF)
            return;

        for (uint64 device = 0; device < 32; device++) {
            EnumerateDevice(busAddress, device);
        }
    }

    void EnumeratePCI(ACPI::MCFGHeader* mcfg) 
    {
        // Amount of device configs avaliable
        int entries = ((mcfg->Length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);

        for (int t = 0; t < entries; t++) {
            ACPI::DeviceConfig* newDeviceConfig =
            (ACPI::DeviceConfig*)((uint64)mcfg + sizeof(ACPI::MCFGHeader)) +
            (sizeof(ACPI::DeviceConfig) *  t);
            for (uint64 bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++) {
                EnumerateBus(newDeviceConfig->BaseAddress, bus);
            }
        }
    }
} // namespace PCI
