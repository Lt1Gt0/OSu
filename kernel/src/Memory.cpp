#include <memory.h>

uint64 GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64 mMapEntries, uint64 mMapDescSize)
{
    static uint64 memorySizeBytes = 0;
    if (memorySizeBytes > 0)
        return memorySizeBytes; // Memory has already been calculated

    //Loop through each map entry and increment the sizebytes by the size of each entry
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64)mMap + (i * mMapDescSize));
        memorySizeBytes += desc->numPages * 4096; //Add the map size in bytes
    }

    return memorySizeBytes;
}

void memset(void* start, uint8 value, uint64 num)
{
    for (uint64 i = 0; i < num; i++) {
        *(uint8*)((uint64)start + i) = value;
    }
}