#include <interrupts/interrupts.h>

#include <renderer.h>
#include <panic.h>
#include <IO.h>
#include <userinput/keyboard.h>
#include <userinput/mouse.h>
#include <timer/pit/pit.h>
#include <interrupts/pic.h>

/* ---------- EXCEPTION HANDLERS ---------- */

__attribute__((interrupt)) void DivideByZero_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void Debug_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void NMI_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void Breakpoint_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void Overflow_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void BoundRangeExceeded_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void InvalidOpcode_Hander(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void DeviceNotAvaliable_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void DoubleFault_Handler(InterruptFrame* frame)
{
    Panic(frame, "Double Fault Detected");
    while(true);
}

__attribute__((interrupt)) void CoprocessorSegmentOverrun_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void InvalidTSS_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void SegmentNotPresent_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void StackSegmentFault_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void GPFault_Handler(InterruptFrame* frame)
{
    Panic(frame, "General Protection Fault Detected");
    while(true);
    PIC::EndMaster();
}

__attribute__((interrupt)) void PageFault_Handler(InterruptFrame* frame)
{
    Panic(frame, "Page Fault Detected");
    while(true);
}

__attribute__((interrupt)) void x87FloatingPointException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void AlignmentCheck_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void MachineCheck_Handler(InterruptFrame* frame)
{
	Panic(frame, "Machine Check");
	while(true);
}

__attribute__((interrupt)) void SIMDFloatingPointException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void VirtualizationException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void ControlProtectionException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void HypervisorInjectionException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void VMMCommunicationException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void SecurityException_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void TripleFault_Handler(InterruptFrame* frame)
{

}

__attribute__((interrupt)) void FPIErrorInterrupt_Handler(InterruptFrame* frame)
{

}

/* ---------- IRQS ---------- */   

__attribute__((interrupt)) void KeyboardInt_Handler(InterruptFrame* frame)
{
    uint8 scancode = inb(0x60);
    HandlerKeyboard(scancode);
    PIC::EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(InterruptFrame* frame)
{
    uint8 mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC::EndSlave();
}

__attribute__((interrupt)) void PITInt_Handler(InterruptFrame* frame)
{
    PIT::Tick();
    PIC::EndMaster();
}
