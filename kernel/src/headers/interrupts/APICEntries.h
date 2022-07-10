#pragma once
#ifndef _APIC_ENTRIES_H
#define _APIC_ENTRIES_H

#include <types.h>

#define PACKED __attribute__((packed));

// https://uefi.org/sites/default/files/resources/ACPI_6_3_final_Jan30.pdf#G10.1360148 (Record structures P.153)
namespace APIC
{
    namespace Record
    {
        struct Header {
            uint8 Type;
            uint8 Length;            
        } PACKED;

        // Type 0
        struct ProcLAPIC : Header {
            uint8   ProcessorID;
            uint8   ID;
            uint32  Flags;
        } PACKED;

        // Type 1
        struct IOAPIC : Header {
            uint8   ID;
            uint8   RESV;
            uint8   Address;
            uint32  GlobalSystemInterruptBase;
        } PACKED;

        // IOAPIC Interrupt Source Override
        // Type 2
        struct IOAPIC_ISO : Header {
            uint8   BusSource;
            uint8   IRQSource;
            uint32  GlobalSystemInterrupt;
            uint16  Flags;
        } PACKED;

        // Type 3    
        struct IOAPIC_NMISource : Header {
            uint8   Source;
            uint8   RESV;
            uint16  Flags;
            uint32  GlobalSystemInterrupt;
        } PACKED;

        // Type 4
        struct LAPIC_NMI : Header {
            uint8   ACPIProcUID;
            uint16  Flags;
            uint8   LINTn; // Local APIC interrupt input LINTn which NMI is connected to
        } PACKED;

        // Type 5
        struct LAPIC_AddressOveride : Header {
            uint16  RESV;
            uint64  PhysicalAddress;
        } PACKED;

        /* 
        types 6 - 15 are NOT implemented as of now, I have no need to implement them
        currently, the structs are just setup for possible future implementation
        */

        // Type 6
        struct IOSAPIC : Header {
            uint8   IOAPIC_ID;
            uint8   RESV;
            uint32  GlobalSystemInterruptBase;
            uint64  Address;
        } PACKED;

        // Type 7
        struct LSAPIC : Header {
            uint8   ACPIProcessorID;
            uint8   ID;
            uint8   EID;
            uint8   RESV[3];
            uint32  Flags;
            uint32  ACPIProcessorUIDValue;
            uint8   ACPIProcessorUIDString;
        } PACKED;
        
        struct PISFlags {
            uint32 CPEIProcessorOverride : 1;
            uint32 RESV : 31;
        } PACKED;

        // Type 8
        struct PIS : Header {
            uint16    Flags;
            uint8     InterruptType;
            uint8     ProcessorID;
            uint8     ProcessorEID;
            uint8     IOSAPICVector;
            uint32    GlobalSystemInterrupt;
            PISFlags  PISflags;
        } PACKED;

        // Type 9
        struct ProcLocalx2APIC : Header {
            uint16  RESV;
            uint32  ID;
            uint32  Flags;
            uint32  ACPI_ID;
        } PACKED;
        
        // Type 10 (0xA)
        struct Lx2APICNMI : Header {
            uint16  Flags;
            uint32  ACPIProcessorUID;
            uint8   LINTn;
            uint8   RESV[3];
        } PACKED;
        
        // Type 11 (0xB)
        struct GICC : Header {
            uint16  RESV0;
            uint32  CPUInterfaceNumber;
            uint32  ACPIProcessorUID;
            uint32  Flags;
            uint32  ParkingProtocolVersion;
            uint32  PerformanceInterruptGSIV;
            uint64  ParkedAddress;
            uint64  PhysicalBaseAddress;
            uint64  GICV;
            uint64  GICH;
            uint32  VGICMaintenanceInterrupt;
            uint64  GICRBaseAddress;
            uint64  MPIDR;
            uint8   ProcessorPowerEfficiencyClass;
            uint8   RESV1;
            uint16  SPEOverflowInterrupt;
        } PACKED;

        // Type 12 (0xC)
        struct GICD : Header {
            uint16  RESV0;
            uint32  GICID;
            uint64  PhysicalBaseAddress;
            uint32  SystemVectorBase;
            uint8   GICVersion;
            uint8   RESV1[3];
        } PACKED;

        // Type 13 (0xD)
        struct GICMSIFrame : Header {
            uint16 RESV;
            uint32 GICMSIFrameID;
            uint64 PhysicalBaseAddress;
            uint32 Flags;
            uint16 SPICount;
            uint16 SPIBase;
        } PACKED;

        // Type 14 (0xE)
        struct GICR : Header {
            uint16 RESV;
            uint64 DiscoveryRangeBaseAddress;
            uint32 DiscoveryRangeLength;
        } PACKED;
        
        // Type 15 (0xF)
        struct GICITS : Header {
            uint16 RESV0;
            uint32 ID;
            uint64 PhysicalBaseAddress;
            uint32 RESV1;
        } PACKED;
    }

    namespace RecordType
    {
        enum : uint8 {
            ProcLAPIC               = 0x0,
            IOAPIC                  = 0x1,
            IOAPIC_ISO              = 0x2,
            IOAPIC_NMISource        = 0x3,
            LAPIC_NMI               = 0x4,
            LAPIC_AddressOveride    = 0x5,
            IOSAPIC                 = 0x6,
            LSAPIC                  = 0x7,
            PIS                     = 0x8,
            ProcLocalx2APIC         = 0x9,
            Lx2APICNMI              = 0xA,
            GICC                    = 0xB,
            GICD                    = 0xC,
            GICMSIFrame             = 0xD,
            GICR                    = 0xE,
            GICITS                  = 0xF
        };
    }
}

#undef PACKED
#endif // _APIC_ENTRIES_H