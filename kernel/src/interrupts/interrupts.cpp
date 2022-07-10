#include <interrupts/interrupts.h>

#include <BasicRenderer.h>
#include <panic.h>
#include <IO.h>
#include <userinput/keyboard.h>
#include <userinput/mouse.h>
#include <timer/pit/pit.h>
#include <interrupts/PIC.h>

/*** EXCEPTIONS ***/

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

/* Should mainly occur when the page table/directory is not in phsycial memory */
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame)
{
    Panic("Page Fault Detected");
    while(true);
}

/* exception is unhandler or
 * exception occurs when the cpu is trying to call an exception handler */
__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame)
{
    Panic("Double Fault Detected");
    while(true);
}

/* Mainly caused because of segment error dealing with privlige levels */
__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame)
{
    Panic("General Protection Fault Detected");
    while(true);
    PIC::EndMaster();
}

/*** IRQ ***/

/* Keyboard press (PS2 keyboard) */
__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame)
{
    uint8 scancode = inb(0x60);
    HandlerKeyboard(scancode);
    PIC::EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame)
{
    uint8 mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC::EndSlave();
}

__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame)
{
    PIT::Tick();
    PIC::EndMaster();
}