#pragma once
#ifndef _APIC_ENTRIES_H
#define _APIC_ENTRIES_H

#include <stdint.h>

#define PACKED __attribute__((packed));

namespace APIC
{
    namespace Record
    {
        struct Header {
            uint8_t Type;
            uint8_t Length;            
        } PACKED;

        struct ProcLAPIC {
            Header      header;
            uint8_t     ProcessorID;
            uint8_t     ID;
            uint32_t    Flags;
        } PACKED;

        struct IOAPIC {
            Header      header;
            uint8_t     ID;
            uint8_t     RESV;
            uint8_t     Address;
            uint32_t    GlobalSystemInterruptBase;
        } PACKED;

        // IOAPIC Interrupt Source Override
        struct IOAPIC_ISO {
            Header      header;
            uint8_t     BusSource;
            uint8_t     IRQSource;
            uint32_t    GlobalSystemInterrupt;
            uint16_t    Flags;
        } PACKED;

        struct IOAPIC_NMISource {
            Header      header;
            uint8_t     Source;
            uint8_t     RESV;
            uint16_t    Flags;
            uint32_t    GlobalSystemInterrupt;
        } PACKED;

        struct LAPIC_AddressOveride {
            Header      header;
            uint16_t    RESV;
            uint64_t    PhysicalAdress;
        } PACKED;

        // For x2APIC I will include the struct but currently do
        // nothing to implement support for it, just know its here
        // if I ever decide to work on the x2APIC
        struct ProcLocalx2APIC {
            Header      header;
            uint16_t    RESV;
            uint32_t    ID;
            uint32_t    Flags;
            uint32_t    ACPI_ID;
        } PACKED;
    }
}

#endif // _APIC_ENTRIES_H