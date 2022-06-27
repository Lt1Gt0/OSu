#include <interrupts/APIC.h>

#include <acpi.h>
#include <paging/PageFrameAllocator.h>

namespace APIC
{
    IA32_APIC_BASE_REG* LoadLAPICData()
    {
        IA32_APIC_BASE_REG regData;

        __asm__("xor %rcx, %rcx");
        __asm__("mov $0x1B, %rcx");
        __asm__("rdmsr" : : "g" (regData));
        
        uint64_t baseAddr = (uint64_t)regData.BaseAddress;
        GlobalAllocator.LockPage((void*)baseAddr); 

        return &regData;
    }
}