#pragma once
#include "Typedefs.h"
#include "Memory.h"

struct MemorySegmentHeader{
    uint64 MemoryLength;
    MemorySegmentHeader* NextSegment;
    MemorySegmentHeader* PreviousSegment;
    MemorySegmentHeader* NextFreeSegment;
    MemorySegmentHeader* PreviousFreeSegment;
    bool Free;
};

void InitializeHeap(uint64 heapAddress, uint64 heapLength);
void* calloc(uint64 size);
void* malloc(uint64 size);
void* realloc(void* address, uint64 newSize);
void free(void* address);