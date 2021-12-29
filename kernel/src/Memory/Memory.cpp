#include "Memory.h"

void memset(void* start, uint64 value, uint64 num){
    if(num <= 8){
        uint8* valPtr = (uint8*)&value;
        for(uint8* ptr = (uint8*)start; ptr < (uint8*)((uint64)start + num); ptr++){
            *ptr = *valPtr;
            valPtr++;
        }
        return;
    }

    uint64 proceedingBytes = num % 8;
    uint64 newNum = num - proceedingBytes;

    for(uint64* ptr = (uint64*)start; ptr < (uint64*)((uint64)start + newNum); ptr++){
        *ptr = value;
    }

    uint8* valPtr = (uint8*)&value;
    for(uint8* ptr = (uint8*)((uint64)start + newNum); ptr < (uint8*)((uint64)start + num); ptr++){
        *ptr = *valPtr;
        valPtr++;
    }
}

void memcpy(void* destination, void* source, uint64 num){
    if(num <= 8){
        uint8* valPtr = (uint8*)&source;
        for(uint8* ptr = (uint8*)destination; ptr < (uint8*)((uint64)destination + num); ptr++){
            *ptr = *valPtr;
            valPtr++;
        }
    }

    uint64 proceedingBytes = num % 8;
    uint64 newNum = num - proceedingBytes;
    uint64* srcPtr = (uint64*)source;

    for(uint64* destPtr = (uint64*)destination; destPtr < (uint64*)((uint64)destination + newNum); destPtr++){
        *destPtr = *srcPtr;
        srcPtr++;
    }

    uint8* srcPtr8 = (uint8*)((uint64)source + newNum);
    for(uint8* destPtr8 = (uint8*)((uint64)destination + newNum); destPtr8 < (uint8*)((uint64)destination + num); destPtr8++){
        *destPtr8 = *srcPtr8;
        srcPtr8++;
    }
}