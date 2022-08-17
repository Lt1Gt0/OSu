#include <interrupts/lapic.h>

#include <interrupts/apic.h>
#include <interrupts/apicEntries.h>
#include <acpi.h>
#include <paging/pageframeAllocator.h>
#include <renderer.h>
#include <interrupts/pic.h>

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
