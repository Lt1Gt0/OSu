#include <interrupts/LAPIC.h>

#include <acpi.h>
#include <paging/PageFrameAllocator.h>
#include <BasicRenderer.h>
#include <interrupts/PIC.h>
#include <cstr.h>
#include <stdint.h>

namespace LAPIC
{
    void Enable(BootInfo* bootInfo)
    {
        // PIC::Disable();
        ParseMADT(bootInfo->rsdp);

        // Map
    }
    
    void ParseMADT(ACPI::RSDP2* rsdp)
    {
        ACPI::XSDTHeader* xsdt = (ACPI::XSDTHeader*)(rsdp->XSDTAddress);
        ACPI::MADTHeader* madt = (ACPI::MADTHeader*)ACPI::FindTable(xsdt, (char*)"APIC");

        GlobalRenderer.Print("MADT Local APIC Address: 0x");
        GlobalRenderer.PrintLine(to_hstring(madt->LocalAPICAddress));
    }


}