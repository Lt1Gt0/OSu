#include <interrupts/APIC.h>

#include <acpi.h>
#include <cstr.h>

namespace APIC
{
    void DetectCores(uint8_t* rsdt)
    {
        uint8_t *ptr, *ptr2;
        uint32_t len;

        for (len = *((uint32_t*)(rsdt + 4)), ptr2 = rsdt + 36; ptr2 < rsdt + len; ptr2 += (rsdt[0] == 'X') ? 8 : 4) {
            ptr = (uint8_t*)(uintptr_t)((rsdt[0] == 'X') ? *((uint64_t*)ptr2) : *((uint32_t*)ptr2));

            if (!memcmp(ptr, "APIC", 4)) {
                // MADT is found
                lApicPtr = (uint64_t)(*((uint32_t*)(ptr + 0x24)));
                ptr2 = ptr + *((uint32_t*)(ptr + 4));

                // Iterate on variable length records
                for (ptr += 44; ptr < ptr2; ptr += ptr[1]) {
                    switch (ptr[0]) {
                    case 0:
                        if (ptr[4] & 0x01)
                            lApicIds[numcore++] = ptr[3];
                            break;
                    case 1:
                        ioApicPtr = (uint64_t)*((uint32_t*)(ptr + 4));
                        break;
                    case 5:
                        lApicPtr = *((uint64_t*)(ptr + 4));
                        break;
                    }
                }
                
                break;
            }
        }
    }
    
    void LoadHeader()
    {
        ACPI::SDTHeader* madtHeader;
        ACPI::MADT* madt = (ACPI::MADT*) ACPI::FindTable(madtHeader, (char*)"APIC");
    }
}