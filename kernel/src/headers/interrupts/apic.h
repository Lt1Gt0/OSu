#pragma once
#ifndef _APIC_H_
#define _APIC_H_

#include <acpi.h>
#include <interrupts/lapic.h>
#include <types.h>

namespace APIC
{
    constexpr u32 APIC_BASE {0xFEE00000};

    extern LAPIC::LAPIC_REGS* lapic;   

    void Initialize();
    void ParseMADT(ACPI::MADTHeader* madt);
    
    void RegisterIOAPIC(uint8 id, uint32 addr, uint32 gsiBase);
    void WriteIOAPICReg(const uint32, const uint8 offset, const uint32 val);
    uint32 readIOAPICReg(const uint32 apicBase, const uint8 offset); 
}

#endif // _APIC_H_
