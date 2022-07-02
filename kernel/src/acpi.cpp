#include <acpi.h>

namespace ACPI
{
    void* FindTable(XSDTHeader* XSDTHeader, char* signature)
    {
        //Amount of entries in the xsdt
        int entries = (XSDTHeader->Length - sizeof(ACPI::XSDTHeader)) / 8;

        for (int t = 0; t < entries; t++) {
            ACPI::XSDTHeader* newXSDTHeader = (ACPI::XSDTHeader*)*(uint64_t*)((uint64_t)XSDTHeader + sizeof(ACPI::XSDTHeader) + (t * 8));
            for (int i = 0; i < 4; i++) {
                if (newXSDTHeader->Signature[i] != signature[i])
                    break;

                if (i == 3)
                    return newXSDTHeader;
            }
        }
        
        return 0;
    }
}