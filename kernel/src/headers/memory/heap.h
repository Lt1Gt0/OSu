#pragma once
#ifndef _HEAP_H
#define _HEAP_H

#include <types.h>

struct HeapSegHdr {
    size_t      length;
    HeapSegHdr* next;
    HeapSegHdr* last;
    bool        free;
    void        CombineForward();
    void        CombineBackward();
    HeapSegHdr* Split(size_t splitLength);
};

void InitializeHeap(void* heapAddress, size_t pageCount);
void* malloc(size_t size);
void free(void* address);
void ExpandHeap(size_t length);

inline void* operator new(size_t size)
{
    return malloc(size);
}

inline void* operator new[](size_t size)
{
    return malloc(size);
}

inline void operator delete(void* p)
{
    free(p);
}

inline void operator delete[](void* p)
{
    free(p);
}

#endif // _HEAP_H