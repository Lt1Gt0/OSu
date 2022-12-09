#pragma once
#ifndef _PCI_H
#define _PCI_H

#include <stdint.h>
#include <acpi.h>
#include <paging/PageTableManager.h>
#include <BasicRenderer.h>
#include <globals.h>
#include <cstr.h>
#include <memory/heap.h>

namespace PCI
{
    // header specifications from intel
    // https://www.intel.com/content/www/us/en/io/serial-ata/ahci.html

    struct PCIDeviceHeader {
        uint16_t    VendorID;       // Device number assigned by vendor
        uint16_t    DeviceID;       // Company vendor (assigned by PCI SIG)
        uint16_t    Command;        // More info on pg. 16 of AHCI specs (rev1-3-1)
        uint16_t    Status;         // More info on pg. 17 of AHCI specs (rev1-3-1)
        uint8_t     RevisionID;     // Stepping of HBA hardware
        uint8_t     ProgIF;         // Programming Interface (PI) (rev1-3-1)
        uint8_t     Subclass;       // if 06h then this is a SATA device
        uint8_t     Class;          // Indicated that device is a mass storage device
        uint8_t     CacheLineSize;  // CLS for use with mem write and invalidate command. when to use mem read multiple/line/command
        uint8_t     LatencyTimer;   // # of clocks in HBA allowed to act as a master on PCI
        uint8_t     HeaderType;     // 06:00 -> HBA target layout, 07 -> if HBA is part of multi-function device
        uint8_t     BIST;           // (Optional: More info on pg. 18 of AHCI spec rev1-3-1)
    };
    
    struct PCIHeader0 {
        PCIDeviceHeader Header;

        uint32_t        BAR0;
        uint32_t        BAR1;
        uint32_t        BAR2;
        uint32_t        BAR3;
        uint32_t        BAR4;
        uint32_t        BAR5;

        uint32_t        CardbusCISPtr;
        uint16_t        SubsystemVendorID;
        uint16_t        SubsystemID;

        uint32_t        ExpansionROMBaseAddr;
        uint8_t         CapabilitiesPtr;

        uint8_t         Rsv0;
        uint16_t        Rsv1;
        uint32_t        Rsv2;
        
        uint8_t         interruptLine;
        uint8_t         interruptPin;
        uint8_t         MinGrant;
        uint8_t         MaxLatency;
    };

    void EnumeratePCI(ACPI::MCFGHeader* mcfg);
    extern const char* DeviceClasses[];

    const char* GetVendorName(uint16_t vendorID);
    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID);
    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode);
    const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);
}

#endif // _PCI_H