#pragma once
#include "Typedefs.h"
#include "Terminal.hpp"

using namespace Terminal;

namespace MemoryMap{
    struct MemoryMapEntry{
        uint64 BaseAddress;
        uint64 RegionLength;
        uint32 RegionType;
        uint32 ExtendedAttributes;
    };

    extern "C" byte MemoryRegionCount;
    extern "C" MemoryMapEntry* MemoryMapLocation;

    extern "C" bool memoryRegionsGot;
    extern "C" byte usableMemoryRegionCount;

    extern MemoryMapEntry* usableMemoryRegions[10];

    /**
     * @brief Print out the memory map that is given as an input
     * 
     * @param memoryMap memory map entry
     * @param position position of the cursor to print memory map at
     */
    void printMemoryMap(MemoryMapEntry* memoryMap, uint16 position);

    /**
     * @brief Get the Usable Memory Regions object
     * 
     * @return MemoryMapEntry** 
     */
    MemoryMapEntry** getUsableMemoryRegions();
}