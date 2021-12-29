#pragma once
#include "Typedefs.h"

struct MemorySegmentHeader{
    uint64 MemoryLength;
    MemorySegmentHeader* NextSegment;
    MemorySegmentHeader* PreviousSegment;
    MemorySegmentHeader* NextFreeSegment;
    MemorySegmentHeader* PreviousFreeSegment;
    bool Free;
};

extern void InitializeHeap(uint64 heapAddress, uint64 heapLength);
extern void* malloc(uint64 size);