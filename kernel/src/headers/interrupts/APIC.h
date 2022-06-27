#pragma once
#ifndef _APIC_H
#define _APIC_H

#include <stdint.h>

namespace APIC
{
    struct IA32_APIC_BASE_REG {
        uint64_t RESV0          : 8;
        uint64_t BSP            : 1;
        uint64_t RESV1          : 2;
        uint64_t Enabled        : 1;
        uint64_t BaseAddress    : 20; 
        uint64_t RESV2          : 32;
    } __attribute__((packed));

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

    IA32_APIC_BASE_REG* LoadAPICData();
}

#endif // _APIC_H