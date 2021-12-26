#pragma once
// #include "MemoryMap.hpp"
#include "Typedefs.h"
#include "Terminal.hpp"

using namespace Terminal;

struct MemoryMapEntry{
    uint64 BaseAddress;
    uint64 RegionLength;
    uint32 RegionType;
    uint32 ExtendedAttributes;
};

extern byte MemoryRegionCount;

/**
 * @brief Print out the memory map that is given as an input
 * 
 * @param memoryMap memory map entry
 * @param position position of the cursor to print memory map at
 */
void printMemoryMap(MemoryMapEntry* memoryMap, uint16 position){
    Terminal::setCursorPosition(position);
    
    outputString("Memory Base: ");
    outputString(intToString(memoryMap->BaseAddress));
    setCursorPosition(position + VGA_WIDTH);
    
    outputString("Region Length: ");
    outputString(intToString(memoryMap->RegionLength));
    setCursorPosition(position + VGA_WIDTH*2);
    
    outputString("Memory Type: ");
    outputString(intToString(memoryMap->RegionType));
    setCursorPosition(position + VGA_WIDTH*3);
    
    outputString("Memory Attributes: ");
    outputString(hexToString(memoryMap->ExtendedAttributes));
}