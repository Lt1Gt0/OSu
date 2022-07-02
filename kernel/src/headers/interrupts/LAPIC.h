#pragma once
#ifndef _LAPIC_H
#define _LAPIC_H

#include <stdint.h>
#include <kernelUtil.h>

namespace LAPIC
{
    struct Register {
        public:
            volatile uint32_t value;

            inline uint32_t Get()
            {
                uint32_t volatile* ptr = &this->value;
                return *ptr;
            }

            inline void Set(uint32_t value)
            {
                uint32_t volatile* ptr = &this->value;
                *ptr = value;
            }

            inline operator uint32_t() {return this->Get();}
            inline void operator = (uint32_t value) {this->Set(value);}
            inline void operator |= (uint32_t value) {this->Set(this->Get() | value);}
            inline void operator &= (uint32_t value) {this->Set(this->Get() & value);}
        private:
            volatile uint8_t padding[0x10 - 4];
    };

    struct LVTEntry {
        uint16_t EntryVector        : 8;
        uint16_t DeliveryMode       : 2;
        uint16_t DestinationMode    : 1;
        uint16_t DeliveryStatus     : 1;
        uint16_t InterruptPolatiry  : 1;
        uint16_t RemoteIRR          : 1;
        uint16_t TriggerMode        : 1;
        uint16_t InterruptMask      : 1;
    } __attribute__((packed));

    struct LVT {
        LVTEntry Timer;
        LVTEntry ThermalMonitor;
        LVTEntry PerformaceCounterRegister;
        LVTEntry LINT0; 
        LVTEntry LINT1;
        LVTEntry Error; 
    };

    struct LAPIC_REGS {
        uint8_t     RESV0[0x20];
        Register    ID; // (RW)
        Register    Version; // (RO)
        uint8_t     RESV1[0x40];
        Register    TPR; // (RW) Task Priority Register
        Register    APR; // (RO) Arbitration Priority Register
        Register    PPR; // (RO) Processes Priority Register
        Register    EOI; // (WO) EOI Register
        Register    RRD; // (RO) Remote Read Register
        Register    LDR; // (RW) Logical Destination Register
        Register    DFR; // (RW) Destination Format Register
        Register    SIVR; // (RW) Spurious Interrupt Vector Register

        Register    ISR0; // (RO) In-Service Register
        Register    ISR1; // (RO) In-Service Register
        Register    ISR2; // (RO) In-Service Register
        Register    ISR3; // (RO) In-Service Register
        Register    ISR4; // (RO) In-Service Register
        Register    ISR5; // (RO) In-Service Register
        Register    ISR6; // (RO) In-Service Register
        Register    ISR7; // (RO) In-Service Register


        Register    TMR0; // (RO) Trigger Mode Register
        Register    TMR1; // (RO) Trigger Mode Register
        Register    TMR2; // (RO) Trigger Mode Register
        Register    TMR3; // (RO) Trigger Mode Register
        Register    TMR4; // (RO) Trigger Mode Register
        Register    TMR5; // (RO) Trigger Mode Register
        Register    TMR6; // (RO) Trigger Mode Register
        Register    TMR7; // (RO) Trigger Mode Register

        Register    IRR0; // (RO) Interrupt Request Register
        Register    IRR1; // (RO) Interrupt Request Register
        Register    IRR2; // (RO) Interrupt Request Register
        Register    IRR3; // (RO) Interrupt Request Register
        Register    IRR4; // (RO) Interrupt Request Register
        Register    IRR5; // (RO) Interrupt Request Register
        Register    IRR6; // (RO) Interrupt Request Register
        Register    IRR7; // (RO) Interrupt Request Register

        Register    ErrorStatus; // (RO)

        uint8_t     RESV2[0x60];

        Register    CMCI; // (RW) LVT Correct Machine Check Interrupt (CMCI) Register

        Register    ICR0; // (RW) Interrupt Command Register
        Register    ICR1; // (RW) Interrupt Command Register
        
        Register    LVT_Timer; // (RW) LVT Timer Register
        Register    LVT_Thermal; // (RW) LVT Thermal Sensor Register
        Register    LVT_PCM; // (RW) LVT Performance Monitoring Counters Register
        Register    LVT_LINT0; // (RW) LVT LINT0 Register
        Register    LVT_LINT1; // (RW) LVT LINT1 Register
        Register    LVT_Error; // (RW) LVT Error Register
        Register    InitialCount; // (RW) Initial Count Register (For Timer)
        Register    CurrentCount; // (RO) Current Count Register (For Timer)
        uint8_t     RESV3[0x40];
        uint32_t    DivideConfiguration; // (RW) Divide Configuration Register (For Timer)
        uint8_t     RESV4[0x10];
    } __attribute__((packed)); // align each register to 0x10 (16) byte boundry

    extern LAPIC_REGS* LAPIC;
    extern LAPIC_REGS* physLAPIC;

    void Enable(BootInfo* bootInfo);
    void ParseMADT(ACPI::RSDP2* rsdp);
}

#endif // _LAPIC_H