#pragma once
#include "Typedefs.h"

namespace MemoryMap{
    struct MemoryMapEntry{
        uint64 baseAddress;
        uint64 regionLength;
        uint32 regionType;
        uint32 extendedAttributes;
    };

    extern uint8 MemoryRegionCount;
}