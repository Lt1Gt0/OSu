#include <memory/heap.h>

#include <paging/PageTableManager.h>
#include <paging/PageFrameAllocator.h>

void* heapStart;
void* heapEnd;
HeapSegHdr* LastHdr;

void InitializeHeap(void* heapAddress, size_t pageCount)
{
    void* pos = heapAddress;

    for (size_t i = 0; i < pageCount; i++) {
        PageTableManager::MapMemory(pos, GlobalAllocator.RequestPage());
        pos = (void*)((size_t)pos + PAGESIZE);
    }

    size_t heapLength = pageCount * PAGESIZE;
    heapStart = heapAddress;
    heapEnd = (void*)((size_t)heapStart + heapLength);
    HeapSegHdr* startSeg = (HeapSegHdr*)heapAddress;
    startSeg->length = heapLength - sizeof(HeapSegHdr);
    startSeg->next = nullptr;
    startSeg->last = nullptr;
    startSeg->free = true;
    LastHdr = startSeg;
}

void* malloc(size_t size)
{
    if (size % 0x10 > 0) { // Check for multiple of 0x10    
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) 
        return nullptr;

    HeapSegHdr* currentSeg = (HeapSegHdr*)heapStart;
    while (true) {
        // Find free block
        if (currentSeg->free) {
            // Mark as used
            if (currentSeg->length > size) {
                currentSeg->Split(size);
                currentSeg->free = false;
                return (void*)((uint64)currentSeg + sizeof(HeapSegHdr));
            }

            if (currentSeg->length == size) {
                currentSeg->free = false;
                return (void*)((uint64)currentSeg + sizeof(HeapSegHdr));
            }
        }

        if (currentSeg->next == nullptr) 
            break;

        currentSeg = currentSeg->next;
    }

    ExpandHeap(size);
    return malloc(size);
}

HeapSegHdr* HeapSegHdr::Split(size_t splitLength)
{
    if (splitLength < 0x10) 
        return nullptr;

    int64 splitSegLength = length - splitLength - (sizeof(HeapSegHdr));
    
    if (splitSegLength < 0x10) 
        return nullptr;

    HeapSegHdr* newSplitHdr = (HeapSegHdr*)((size_t)this + splitLength + sizeof(HeapSegHdr));
    next->last = newSplitHdr; // set the next segment's last segment to new segment
    newSplitHdr->next = next; // set the new segment's next segment to original next segment
    next = newSplitHdr; // set next segment to new segment
    newSplitHdr->last = this; // set new segment's last segment to current segment
    newSplitHdr->length = splitSegLength; // set the new header's length to the calculated value
    newSplitHdr->free = free; // make sure the new segment's free is the same as the original
    length = splitLength; // set the length of the original segment to its new length

    if (LastHdr == this) 
        LastHdr = newSplitHdr;
    
    return newSplitHdr;
}

void ExpandHeap(size_t length)
{
    if (length % PAGESIZE) {
        length -= length % PAGESIZE;
        length += PAGESIZE;
    }

    size_t pageCount = length / PAGESIZE;
    HeapSegHdr* newSegment = (HeapSegHdr*)heapEnd;

    for (size_t i = 0; i < pageCount; i++) {
        PageTableManager::MapMemory(heapEnd, GlobalAllocator.RequestPage());
        heapEnd = (void*)((size_t)heapEnd + PAGESIZE);
    }

    newSegment->free = true;
    newSegment->last = LastHdr;
    LastHdr->next = newSegment;
    LastHdr = newSegment;
    newSegment->next = nullptr;
    newSegment->length = length - sizeof(HeapSegHdr);
    newSegment->CombineBackward();
}

void free(void* address)
{
    HeapSegHdr* segment = (HeapSegHdr*)address - 1;
    segment->free = true;
    segment->CombineForward();
    segment->CombineBackward();
}

void HeapSegHdr::CombineForward()
{
    if (next == nullptr) 
        return;

    if (!next->free)
        return;

    if (next == LastHdr)
        LastHdr = this;

    if (next->next != nullptr)
        next->next->last = this;

    length += next->length + sizeof(HeapSegHdr);

    next = next->next;
}

void HeapSegHdr::CombineBackward()
{
    if (last != nullptr && last->free)
        last->CombineForward();
}