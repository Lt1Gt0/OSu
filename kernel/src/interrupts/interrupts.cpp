#include <interrupts/interrupts.h>

#include <renderer.h>
#include <panic.h>
#include <IO.h>
#include <userinput/keyboard.h>
#include <userinput/mouse.h>
#include <timer/pit/pit.h>
#include <interrupts/PIC.h>

/* ---------- EXCEPTION HANDLERS ---------- */

__attribute__((interrupt)) void DivideByZero_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void Debug_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void NMI_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void Breakpoint_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void Overflow_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void BoundRangeExceeded_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void InvalidOpcode_Hander(interruptFrame* frame)
{

}

__attribute__((interrupt)) void DeviceNotAvaliable_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void DoubleFault_Handler(interruptFrame* frame)
{
    Panic("Double Fault Detected");
    while(true);
}

__attribute__((interrupt)) void CoprocessorSegmentOverrun_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void InvalidTSS_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void SegmentNotPresent_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void StackSegmentFault_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void GPFault_Handler(interruptFrame* frame)
{
    Panic("General Protection Fault Detected");
    while(true);
    PIC::EndMaster();
}

__attribute__((interrupt)) void PageFault_Handler(interruptFrame* frame)
{
    Panic("Page Fault Detected");
    while(true);
}

__attribute__((interrupt)) void x87FloatingPointException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void AlignmentCheck_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void MachineCheck_Handler(interruptFrame* frame)
{
	Panic("Machine Check");
	while(true);
}

__attribute__((interrupt)) void SIMDFloatingPointException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void VirtualizationException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void ControlProtectionException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void HypervisorInjectionException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void VMMCommunicationException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void SecurityException_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void TripleFault_Handler(interruptFrame* frame)
{

}

__attribute__((interrupt)) void FPIErrorInterrupt_Handler(interruptFrame* frame)
{

}

/* ---------- IRQS ---------- */   

__attribute__((interrupt)) void KeyboardInt_Handler(interruptFrame* frame)
{
    uint8 scancode = inb(0x60);
    HandlerKeyboard(scancode);
    PIC::EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(interruptFrame* frame)
{
    uint8 mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC::EndSlave();
}

__attribute__((interrupt)) void PITInt_Handler(interruptFrame* frame)
{
    PIT::Tick();
    PIC::EndMaster();
}
