#pragma once
#ifndef _LAPIC_H
#define _LAPIC_H

#include <stdint.h>

namespace LAPIC
{
    #define IA32_APIC_BASE_MSR          0x1B
    #define IA32_APIC_BASE_MSR_BSP      0x100
    #define IA32_APIC_BASE_MSR_ENABLE   0x800

    // struct IA32_APIC_BASE_MSR {
    //     uint64_t RESV0          : 8;
    //     uint64_t BSP            : 1;
    //     uint64_t RESV1          : 2;
    //     uint64_t Enabled        : 1;
    //     uint64_t BaseAddress    : 24; 
    //     uint64_t RESV2          : 28;
    // } __attribute__((packed))
    //   __attribute__((aligned(0x1000)));

    struct LocalVectorTableEntry {
        uint16_t EntryVector        : 8;
        uint16_t DeliveryMode       : 2;
        uint16_t DestinationMode    : 1;
        uint16_t DeliveryStatus     : 1;
        uint16_t InterruptPolatiry  : 1;
        uint16_t RemoteIRR          : 1;
        uint16_t TriggerMode        : 1;
        uint16_t InterruptMask      : 1;
    } __attribute__((packed));

    struct LocalVectorTable {
        LocalVectorTableEntry Timer;
        LocalVectorTableEntry ThermalMonitor;
        LocalVectorTableEntry PerformaceCounterRegister;
        LocalVectorTableEntry LINT0; 
        LocalVectorTableEntry LINT1;
        LocalVectorTableEntry Error; 
    };

    void Detect();
    void GetMSR(uint32_t* lo, uint32_t* hi);
    void SetMSR(uint32_t* lo, uint32_t* hi);
    uintptr_t GetBase();
    void Enable();
}

#endif // _LAPIC_H