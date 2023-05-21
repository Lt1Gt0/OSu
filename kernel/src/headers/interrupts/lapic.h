#pragma once
#ifndef _LAPIC_H_
#define _LAPIC_H_

#include <types.h>
#include <kernelUtil.h>

namespace APIC
{
    namespace LAPIC
    {
        struct Register {
            public:
                volatile uint32 value;

                inline uint32 Get()
                {
                    uint32 volatile* ptr = &this->value;
                    return *ptr;
                }

                inline void Set(uint32 value)
                {
                    uint32 volatile* ptr = &this->value;
                    *ptr = value;
                }

                inline operator uint32() {return this->Get();}
                inline void operator = (uint32 value) {this->Set(value);}
                inline void operator |= (uint32 value) {this->Set(this->Get() | value);}
                inline void operator &= (uint32 value) {this->Set(this->Get() & value);}

            private:
                volatile uint8 padding[0x10 - 4];
        };

        struct LVTEntry {
            uint16 EntryVector        : 8;
            uint16 DeliveryMode       : 2;
            uint16 DestinationMode    : 1;
            uint16 DeliveryStatus     : 1;
            uint16 InterruptPolatiry  : 1;
            uint16 RemoteIRR          : 1;
            uint16 TriggerMode        : 1;
            uint16 InterruptMask      : 1;
        } PACKED;

        struct LVT {
            LVTEntry Timer;
            LVTEntry ThermalMonitor;
            LVTEntry PerformaceCounterRegister;
            LVTEntry LINT0; 
            LVTEntry LINT1;
            LVTEntry Error; 
        };

        struct LAPIC_REGS {
            uint8       RESV0[0x20];
            Register    ID; // (RW)
            Register    Version; // (RO)
            uint8       RESV1[0x40];
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

            uint8       RESV2[0x60];

            Register    CMCI; // (RW) LVT Correct Machine Check Interrupt (CMCI) Register

            Register    ICR0; // (RW) Interrupt Command Register
            Register    ICR1; // (RW) Interrupt Command Register

            Register    LVTimer; // (RW) LVT Timer Register
            Register    LVThermal; // (RW) LVT Thermal Sensor Registerkernel/src/headers/interrupts/LAPIC.h
            Register    LVT_PCM; // (RW) LVT Performance Monitoring Counters Register
            Register    LVT_LINT0; // (RW) LVT LINT0 Register
            Register    LVT_LINT1; // (RW) LVT LINT1 Register
            Register    LVT_Error; // (RW) LVT Error Register

            Register    InitialCount; // (RW) Initial Count Register (For Timer)
            Register    CurrentCount; // (RO) Current Count Register (For Timer)
            uint8       RESV3[0x40];
            Register    DivideConfiguration; // (RW) Divide Configuration Register (For Timer)
            uint8       RESV4[0x10];
        };

        void Enable();
        void Disable();
        void Initialize();
    }
}
#endif // _LAPIC_H_
