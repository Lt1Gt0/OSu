#include <interrupts/LAPIC.h>

#include <interrupts/APIC.h>
#include <interrupts/APICEntries.h>
#include <acpi.h>
#include <paging/PageFrameAllocator.h>
#include <renderer.h>
#include <interrupts/PIC.h>

namespace APIC
{
    namespace LAPIC
    {
        void Enable()
        {
            if (lapic)
                lapic->SIVR = 0x1FF;
        }

        void Disable()
        {
            if (lapic)
                lapic->SIVR = 0;
        }


    }
}
