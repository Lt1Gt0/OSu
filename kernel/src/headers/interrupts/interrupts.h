#pragma once
#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <types.h>

struct PACKED InterruptFrame {
    u64 rip = 0x00;
    u64 cs = 0x00;
    u64 rflags = 0x00;
    u64 rsp = 0x00;
    u64 ss = 0x00;
};

/* ---------- EXCEPTION HANDLERS ---------- */
// Divide By zero (0x00 | fault | DE)
// Debug (0x01 | fault/trap | DB)
// Non-maskable Interrupt (0x02 | Interrupt | -)
// Breakpoint (0x03 | Trap | BP)
// Overflow (0x04 | Trap | OF)
// Bound Range Exceeded (0x05 | Fault | BR)
// Invalid Opcode (0x06 | Fault | UD)
// Device Not avaliable (0x07 | FAULT | NM)
// Double Fault (0x08 | Abort | DF | Error Code 0)
// Coprocessor Segment Overrun (0x09 | Fault | - )
// Invalid TSS (0x0A | Fault | TS | Error Code)
// Segment Not Present (0x0B | Fault | NP | Error Code)
// Stack-Segment Fault (0x0C | Fault | SS | Error Code)
// General Protection Fault (0x0D | Fault | GP | Error Code)
// Page Fault (0x0E | Fault | PF | Error Code)
// RESV (0x0F)
// x87 Floating-Point Exception (0x10 | Fault | MF)
// Alignment Check (0x11 | Fault | AC | Error Code)
// Machine Check (0x12 | Abort | MC)
// SIMD Floating-Point Exception (0x13 | Fault | XM/XF)
// Virtualization Exception (0x14 | Fault | VE)
// Control Protection Exception (0x15 | Fault | CP | Error Code)
// RESV (0x16 - 0x1B)
// Hypervisor Injection Exception (0x1C | Fault | HV)
// VMM Communication Exception (0x1D | Fault | VS | Error Code)
// Security Exception (0x1E | Fault | SX | Error Code)
// RESV (0x1F)
// Triple Fault (-)
// FPU Error Interrupt (IRQ 13 | Interrupt | FEER)

__attribute__((interrupt)) void DivideByZero_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void Debug_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void NMI_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void Breakpoint_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void Overflow_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void BoundRangeExceeded_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void InvalidOpcode_Hander(InterruptFrame* frame);
__attribute__((interrupt)) void DeviceNotAvaliable_Handler(InterruptFrame* frame);

/* exception is unhandled or
 * exception occurs when the cpu is trying to call an exception handler */
__attribute__((interrupt)) void DoubleFault_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void CoprocessorSegmentOverrun_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void InvalidTSS_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void SegmentNotPresent_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void StackSegmentFault_Handler(InterruptFrame* frame);

/* Mainly caused because of segment error dealing with privlige levels */
__attribute__((interrupt)) void GPFault_Handler(InterruptFrame* frame);

/* Should mainly occur when the page table/directory is not in phsycial memory */
__attribute__((interrupt)) void PageFault_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void x87FloatingPointException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void AlignmentCheck_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void MachineCheck_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void SIMDFloatingPointException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void VirtualizationException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void ControlProtectionException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void HypervisorInjectionException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void VMMCommunicationException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void SecurityException_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void TripleFault_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void FPIErrorInterrupt_Handler(InterruptFrame* frame);

/* ---------- IRQS ---------- */   

/* Keyboard press (PS2 keyboard) */
__attribute__((interrupt)) void KeyboardInt_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void MouseInt_Handler(InterruptFrame* frame);
__attribute__((interrupt)) void PITInt_Handler(InterruptFrame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();

#endif // _INTERRUPTS_H_
