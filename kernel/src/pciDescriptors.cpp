#include <stdint.h>
#include "cstr.h"

namespace PCI{
    const char* DeviceClasses[]{
        "Unclassified",
        "Mass Storage Controller",
        "Network Controller",
        "Display Controller",
        "Mutimedia Controller",
        "Memory Controller",
        "Bridge Device",
        "Simple Communication Controller",
        "Docking Station",
        "Processor",
        "Serial Bus Controller",
        "Wireless Controller",
        "Intelligent Controller",
        "Satellite Communication Controller",
        "Encryption Controller",
        "Signal Processing Controller",
        "Processing Accelerator",
        "Non Essential Instrumentation"
    };

    const char* GetVendorName(uint16_t vendorID){
        switch(vendorID){
            case 0x8086: return "Intel Corp";
            case 0x1022: return "AMD";
            case 0x10DE: return "NVIDEA Corporation";
        }
        return to_hstring(vendorID);
    }

    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID){
        switch(vendorID){
            case 0x8086: // Intel
                switch(deviceID){
                    case 0x29C0: return "Express DRAM Controller";
                    case 0x2918: return "LPC Interface Controller";
                    case 0x2922: return "6 Port SATA Controller [AHCI mode]";
                    case 0x2930: return "SMBus Controller";
                }
            case 0x1022: // AMD
                break;
            case 0x10DE: // NVIDEA Corporation
                break;
        }
        return to_hstring(deviceID);
    }

    const char* UnclassifiedSubclassName(uint8_t subclassCode) {
        switch(subclassCode) {
            case 0x00: return "Non-VGA-Compatible Unclassified Device";
            case 0x01: return "VGA-Compatible Unclassified Device";
        }
        return to_hstring(subclassCode);
    }

    const char* MassStorageControllerSubclassName(uint8_t subclassCode){
        switch(subclassCode){
            case 0x00: return "SCSI Buss Controller";
            case 0x01: return "IDE Controller";
            case 0x02: return "Floppy Disk Controller";
            case 0x03: return "IPI Bus Controller";
            case 0x04: return "RAID Controller";
            case 0x05: return "ATA Controller";
            case 0x06: return "Serial ATA";
            case 0x07: return "Serial Attached SCSI";
            case 0x08: return "Non-Volatile Memory Controller";
            case 0x80: return "Other";
        }
        return to_hstring(subclassCode);
    }

    const char* MemoryControllerSubclassName(uint8_t subclassCode) {
        switch(subclassCode){
            case 0x0: return "RAM Controller";
            case 0x1: return "Flash Controller";
            case 0x80: return "Other";
        }
        return to_hstring(subclassCode);
    }

    const char* BridgeControllerSubclassName(uint8_t subclassCode) {
        switch(subclassCode){
            case 0x00: return "Host Bridge";
            case 0x01: return "ISA Bridge";
            case 0x02: return "EISA Beidge";
            case 0x03: return "MCA Bridge";
            case 0x04: return "PCI-to-PCI Bridge";
            case 0x05: return "PCMCIA Bridge";
            case 0x06: return "NuBus Bridge";
            case 0x07: return "CardBus Bridge";
            case 0x08: return "RACEway Bridge";
            case 0x09: return "PCI-to-PCI Bridge";
            case 0x0A: return "InfiniBand-to-PCI Host Bridge";
            case 0x80: return "Other";
        }
        return to_hstring(subclassCode);
    }

    const char* IntelligentControllerSubclassName(uint8_t subclassCode){
        switch(subclassCode){
            case 0x00: return "I20";
        }
        return to_hstring(subclassCode);
    }

    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode) {
        switch(classCode){
            case 0x00: return UnclassifiedSubclassName(subclassCode); 
            case 0x01: return MassStorageControllerSubclassName(subclassCode);
            case 0x05: return MemoryControllerSubclassName(subclassCode);
            case 0x06: return BridgeControllerSubclassName(subclassCode);
            case 0x0E: return IntelligentControllerSubclassName(subclassCode);
            /* 0x14 -> 0x3C (Reserved) */
            /* 0x41 -> 0xFE (Reserved) */
        }
        return to_hstring(subclassCode);
    }

    const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF){
        switch(classCode){
            case 0x00: // Unclassified 
                break;
            case 0x01: // MSC
                switch(subclassCode){
                    case 0x06: // Serial ATA Controller
                        switch(progIF){
                            case 0x00: return "Vendor Specific Interface";
                            case 0x01: return "AHCI 1.0";
                            case 0x02: return "Serial Storage Bus";
                        }
                }
            case 0x05: break; // Memory Controller (To my knowledge there are no Program Interfaces provided)
            case 0x06: // Bridge Controller
                switch(progIF){
                    case 0x00: return "Normal Decode";
                    case 0x01: return "Subtractive Decode"; 
                }
            case 0x0E: break; // Intelligent Controller (To my knowledge there are no Program Interfaces provided)

            /* 0x14 -> 0x3C (Reserved) */
            /* 0x41 -> 0xFE (Reserved) */
        }
        return to_hstring(progIF);
    }
}