#include <interrupts/interrupts.h>

#include <BasicRenderer.h>
#include <panic.h>
#include <IO.h>
#include <userinput/keyboard.h>
#include <userinput/mouse.h>
#include <scheduling/pit/pit.h>
#include <interrupts/PIC.h>

/*** EXCEPTIONS ***/

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
    uint8_t scancode = inb(0x60);
    HandlerKeyboard(scancode);
    PIC::EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame)
{
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC::EndSlave();
}

__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame)
{
    PIT::Tick();
    PIC::EndMaster();
}