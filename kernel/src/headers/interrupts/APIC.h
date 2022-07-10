#pragma once
#ifndef _APIC_H
#define _APIC_H

#include <acpi.h>
#include <interrupts/LAPIC.h>

namespace APIC
{
    extern LAPIC::LAPIC_REGS* lapic;   

    void ParseMADT(ACPI::MADTHeader* madt);
}

#endif // _APIC_H