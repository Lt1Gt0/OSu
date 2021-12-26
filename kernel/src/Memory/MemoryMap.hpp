// #pragma once
// #include "Typedefs.h"
// #include "../Terminal.cpp"

// namespace MemoryMap{
//     struct MemoryMapEntry{
//         uint64 BaseAddress;
//         uint64 RegionLength;
//         uint32 RegionType;
//         uint32 ExtendedAttributes;
//     };
    
//     /**
//      * @brief Print out the memory map that is given as an input
//      * 
//      * @param memoryMap memory map entry
//      * @param position position of the cursor to print memory map at
//      */
//     void printMemoryMap(MemoryMapEntry* memoryMap, uint16 position){
//         Terminal::setCursorPosition(position);
        
//         Terminal::outputString("Memory Base: ");
//         Terminal::outputString(Terminal::intToString(memoryMap->BaseAddress));
//         Terminal::setCursorPosition(position + VGA_WIDTH);

//         Terminal::outputString("Region Length: ");
//         Terminal::outputString(Terminal::intToString(memoryMap->RegionLength));
//         Terminal::setCursorPosition(position + VGA_WIDTH*2);
        
//         Terminal::outputString("Memory Type: ");
//         Terminal::outputString(Terminal::intToString(memoryMap->RegionType));
//         Terminal::setCursorPosition(position + VGA_WIDTH*3);
        
//         Terminal::outputString("Memory Attributes: ");
//         Terminal::outputString(Terminal::hexToString(memoryMap->ExtendedAttributes));
//     }
// }