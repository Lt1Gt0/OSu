#include <interrupts/LAPIC.h>

#include <acpi.h>
#include <paging/PageFrameAllocator.h>
#include <BasicRenderer.h>
#include <cstr.h>
#include <stdint.h>

namespace LAPIC
{
    // IA32_APIC_BASE_REG* LoadAPICData()
    // {
    //     IA32_APIC_BASE_REG regData;

    //     __asm__("mov $0x1B, %rcx");
    //     __asm__("rdmsr" : : "g" (regData));
        
    //     return &regData;
    // }

    void GetMSR(uint32_t* lo, uint32_t* hi)
    {
        asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(IA32_APIC_BASE_MSR));
    }

    void SetMSR(uint32_t lo, uint32_t hi)
    {
        asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(IA32_APIC_BASE_MSR));
    }

    uintptr_t GetBase()
    {
        uint32_t eax, edx;
        GetMSR(&eax, &edx);

        return (eax & 0xFFFFF000);
    }

    void Detect()
    {
        uint32_t edx;
        __asm__("mov $1, %eax");
        __asm__("cpuid");
        __asm__("bt $9, %edx");
        asm volatile("mov %%edx, %[Var]" : [Var] "=r" (edx));

        GlobalRenderer.Print("EDX: 0x");
        GlobalRenderer.PrintLine(to_hstring(edx));
    }

    void Enable()
    {
        
    }
}