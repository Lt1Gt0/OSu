#include "MemoryMap.h"

namespace MemoryMap{
    byte usableMemoryRegionCount;
    bool memoryRegionsGot = false;
    MemoryMapEntry* usableMemoryRegions[10];


    void printMemoryMap(MemoryMapEntry* memoryMap, uint16 position){
        setCursorPosition(position);

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

    MemoryMapEntry** getUsableMemoryRegions(){
        if(memoryRegionsGot) return usableMemoryRegions;

        byte usableRegionIndex;
        for(byte i = 0; i < MemoryRegionCount; i++){
            MemoryMapEntry* MemoryMapLocation = (MemoryMapEntry*)0x500;
            MemoryMapLocation += i;
            if(MemoryMapLocation->RegionType == 1){
                usableMemoryRegions[usableRegionIndex] = MemoryMapLocation;
                usableRegionIndex++;
            }
        }
        usableMemoryRegionCount = usableRegionIndex;
        memoryRegionsGot = true;
        return usableMemoryRegions;
    }
}