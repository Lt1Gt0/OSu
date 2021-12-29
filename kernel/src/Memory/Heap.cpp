#include "Heap.h"

MemorySegmentHeader* FirstFreeMemorySegment;

void InitializeHeap(uint64 heapAddress, uint64 heapLength){
    FirstFreeMemorySegment = (MemorySegmentHeader*)heapAddress;
    FirstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);
    FirstFreeMemorySegment->NextSegment = 0;
    FirstFreeMemorySegment->PreviousSegment = 0;
    FirstFreeMemorySegment->NextFreeSegment = 0;
    FirstFreeMemorySegment->PreviousFreeSegment = 0;
    FirstFreeMemorySegment->Free = true;
}

void* malloc(uint64 size){
    uint64 remainder = size % 8;
    size -= remainder;
    if(remainder != 0) size += 8;

    MemorySegmentHeader* currentMemorySegment = FirstFreeMemorySegment;

    while(true){
        if(currentMemorySegment->MemoryLength >= size){
            if(currentMemorySegment->MemoryLength > size + sizeof(MemorySegmentHeader)){
                MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint64)currentMemorySegment + sizeof(MemorySegmentHeader) + size);

                newSegmentHeader->Free = true;
                newSegmentHeader->MemoryLength = ((uint64)currentMemorySegment->MemoryLength) - (sizeof(MemorySegmentHeader) + size);
                newSegmentHeader->NextFreeSegment = currentMemorySegment->NextFreeSegment;
                newSegmentHeader->NextSegment = currentMemorySegment->NextSegment;
                newSegmentHeader->PreviousSegment = currentMemorySegment;
                newSegmentHeader->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;

                currentMemorySegment->NextFreeSegment = newSegmentHeader;
                currentMemorySegment->NextSegment = newSegmentHeader;
                currentMemorySegment->MemoryLength = size;
            }
            if(currentMemorySegment == FirstFreeMemorySegment){
                FirstFreeMemorySegment = currentMemorySegment->NextFreeSegment;
            }
            currentMemorySegment->Free = false;

            if(currentMemorySegment->PreviousFreeSegment != 0) currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
            if(currentMemorySegment->NextFreeSegment != 0) currentMemorySegment->NextFreeSegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
            if(currentMemorySegment->PreviousFreeSegment != 0) currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
            if(currentMemorySegment->NextFreeSegment != 0) currentMemorySegment->NextSegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
            
            return currentMemorySegment += 1;
        }

        //Should only happen if RAM limit of system is met
        //Ran out of free memory segments; nowhere to allocate
        if(currentMemorySegment->NextFreeSegment == 0){
            return 0; // No memory remaining
        }
        currentMemorySegment = currentMemorySegment->NextFreeSegment;
    }
    return 0; // Should never get here
}

void* calloc(uint64 size){
    void* mallocVal = malloc(size);
    memset(mallocVal, 0, size);
    return mallocVal;
}

void* calloc(uint64 num, uint64 size){
    return calloc(num*size);
}

void* realloc(void* address, uint64 newSize){
    MemorySegmentHeader* oldSegmentHeader = (MemorySegmentHeader*)address - 1;
    uint64 smallerSize = newSize;
    if(oldSegmentHeader->MemoryLength < newSize) smallerSize = oldSegmentHeader->MemoryLength;

    void* newMem = malloc(newSize);
    memcpy(newMem, address, smallerSize);
    free(address);
    return(newMem);
}

void combineFreeSegments(MemorySegmentHeader* a, MemorySegmentHeader* b){
    if(a == 0) return;
    if(b == 0) return;

    if(a < b){
        a->MemoryLength += b->MemoryLength + sizeof(MemorySegmentHeader);
        a->NextSegment = b->NextSegment;
        a->NextFreeSegment = b->NextFreeSegment;
        b->NextSegment->PreviousSegment = a;
        b->NextSegment->PreviousFreeSegment = a;
        b->NextFreeSegment->PreviousFreeSegment = a;
    }
    else{
        b->MemoryLength += a->MemoryLength + sizeof(MemorySegmentHeader);
        b->NextSegment = a->NextSegment;
        b->NextFreeSegment = a->NextFreeSegment;
        a->NextSegment->PreviousSegment = b;
        a->NextSegment->PreviousFreeSegment = b;
        a->NextFreeSegment->PreviousFreeSegment = b;
    }
}

void free(void* address){
    MemorySegmentHeader* currentMemorySegment = ((MemorySegmentHeader*)address) - 1;
    currentMemorySegment->Free = true;

    if(currentMemorySegment < FirstFreeMemorySegment) FirstFreeMemorySegment = currentMemorySegment;

    if(currentMemorySegment->NextFreeSegment != 0){
        if(currentMemorySegment->NextFreeSegment->PreviousFreeSegment < currentMemorySegment)
            currentMemorySegment->NextFreeSegment->PreviousFreeSegment = currentMemorySegment;
    }
    if(currentMemorySegment->PreviousFreeSegment != 0){
        if(currentMemorySegment->PreviousFreeSegment->NextFreeSegment > currentMemorySegment)
            currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment;
    }
    if(currentMemorySegment->NextSegment != 0){
        currentMemorySegment->NextSegment->PreviousFreeSegment = currentMemorySegment;
        if(currentMemorySegment->NextSegment->Free) combineFreeSegments(currentMemorySegment, currentMemorySegment->NextSegment);
    }
    if(currentMemorySegment->PreviousSegment != 0){
        currentMemorySegment->PreviousSegment->NextSegment = currentMemorySegment;
        if(currentMemorySegment->PreviousSegment->Free) combineFreeSegments(currentMemorySegment, currentMemorySegment->PreviousSegment);

    }
}