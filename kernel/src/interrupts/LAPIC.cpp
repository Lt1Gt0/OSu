#include <interrupts/LAPIC.h>

#include <interrupts/APIC.h>
#include <interrupts/APICEntries.h>
#include <acpi.h>
#include <paging/PageFrameAllocator.h>
#include <BasicRenderer.h>
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

        void Initialize()
        {
            PageTableManager::MapMemory(lapic, lapic);
        }
    }
}