#pragma once
#ifndef _PCI_H_
#define _PCI_H_

#include <types.h>
#include <acpi.h>
#include <paging/pagetableManager.h>
#include <string.h>
#include <memory/heap.h>

namespace PCI
{
    // header specifications from intel
    // https://www.intel.com/content/www/us/en/io/serial-ata/ahci.html

    struct PCIDeviceHeader {
        uint16  VendorID;       // Device number assigned by vendor
        uint16  DeviceID;       // Company vendor (assigned by PCI SIG)
        uint16  Command;        // More info on pg. 16 of AHCI specs (rev1-3-1)
        uint16  Status;         // More info on pg. 17 of AHCI specs (rev1-3-1)
        uint8   RevisionID;     // Stepping of HBA hardware
        uint8   ProgIF;         // Programming Interface (PI) (rev1-3-1)
        uint8   Subclass;       // if 06h then this is a SATA device
        uint8   Class;          // Indicated that device is a mass storage device
        uint8   CacheLineSize;  // CLS for use with mem write and invalidate command. when to use mem read multiple/line/command
        uint8   LatencyTimer;   // # of clocks in HBA allowed to act as a master on PCI
        uint8   HeaderType;     // 06:00 -> HBA target layout, 07 -> if HBA is part of multi-function device
        uint8   BIST;           // (Optional: More info on pg. 18 of AHCI spec rev1-3-1)
    };
    
    struct PCIHeader0 : PCIDeviceHeader {
        uint32  BAR0;
        uint32  BAR1;
        uint32  BAR2;
        uint32  BAR3;
        uint32  BAR4;
        uint32  BAR5;

        uint32  CardbusCISPtr;
        uint16  SubsystemVendorID;
        uint16  SubsystemID;

        uint32  ExpansionROMBaseAddr;
        uint8   CapabilitiesPtr;

        uint8   Rsv0;
        uint16  Rsv1;
        uint32  Rsv2;
        
        uint8   interruptLine;
        uint8   interruptPin;
        uint8   MinGrant;
        uint8   MaxLatency;
    };

    void EnumeratePCI(ACPI::MCFGHeader* mcfg);
    extern const char* DeviceClasses[];

    const char* GetVendorName(uint16 vendorID);
    const char* GetDeviceName(uint16 vendorID, uint16 deviceID);
    const char* GetSubclassName(uint8 classCode, uint8 subclassCode);
    const char* GetProgIFName(uint8 classCode, uint8 subclassCode, uint8 progIF);
}

#endif // _PCI_H_
