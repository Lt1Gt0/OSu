#include <paging/pageframeAllocator.h>

#include <memory/memory.h>

uint64 freeMemory; // Free memory of the entire system
uint64 reservedMemory; // Memory reserved by ACPI tables, etc
uint64 usedMemory; // self explanitory
bool Initialized = false;
PageFrameAllocator GlobalAllocator;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize)
{
    if (Initialized)
        return;
    
    Initialized = true;

    void* largestFreeMemSeg = nullptr;
    size_t largestFreeMemSegSize = 0;
    uint64 mMapEntries = mMapSize / mMapDescSize;
    
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64)mMap + (i * mMapDescSize));

        if (desc->type == 7) { // type = EfiConventionalMemory    
            if (desc->numPages * PAGE_SIZE > largestFreeMemSegSize) {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->numPages * PAGE_SIZE;
            }
        }
    }

    uint64 memorySize = GetMemorySize(mMap, mMapEntries, mMapDescSize);
    freeMemory = memorySize; // All current system memory is free

    // Calculate needed size for the bitmap
    uint64 bitmapSize = memorySize / PAGE_SIZE / 8 + 1;

    // Initialize bitmap
    InitBitmap(bitmapSize, largestFreeMemSeg);
    ReservePages(0, memorySize / PAGE_SIZE + 1);

    // Lock pages of bitmap and reserve pages of unusable/reserved memory

    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64)mMap + (i * mMapDescSize));

        if (desc->type == 7) // efiConventionalMemory
            UnreservePages(desc->physAddr, desc->numPages);
    }

    ReservePages(0, 0x100); // Reserve 0->0x100000 for BIOS
    LockPages(this->mPageBitmap.mBuffer, this->mPageBitmap.mSize / PAGE_SIZE + 1);
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    this->mPageBitmap.mSize = bitmapSize;
    this->mPageBitmap.mBuffer = (uint8*)bufferAddress;
    
    for (int i = 0; i < bitmapSize; i++) {
        *(uint8*)(this->mPageBitmap.mBuffer + i)  = 0;
    }
}

uint64 pageBitmapIndex = 0;
void* PageFrameAllocator::RequestPage()
{
    //Subject to future optimization
    for (; pageBitmapIndex < this->mPageBitmap.mSize * 8; pageBitmapIndex++) {
        if (this->mPageBitmap[pageBitmapIndex] == true) 
            continue;
        
        LockPage((void*)(pageBitmapIndex * 0x1000));
        return (void*)(pageBitmapIndex * 0x1000); //Memory address of new free page frame
    }

    return nullptr; //Page frame swap to file
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64 index = (uint64)address / PAGE_SIZE;
    //Check if page is free
    if (this->mPageBitmap[index] == false)
        return;

    if (this->mPageBitmap.Set(index, false)) { // Make free
        freeMemory += PAGE_SIZE;
        usedMemory += PAGE_SIZE;
        if (pageBitmapIndex > index)
            pageBitmapIndex = index;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64 pageCount)
{
    for (int t = 0; t < pageCount; t++) {
        FreePage((void*)((uint64)address + (t * PAGE_SIZE))); // Add bytes of single page and convert back to void*
    }
}

void PageFrameAllocator::LockPage(void* address)
{
    uint64 index = (uint64)address / PAGE_SIZE;

    //Check if page is Locked
    if (this->mPageBitmap[index] == true)
        return;
    
    if (this->mPageBitmap.Set(index, true)) { // Make locked
        freeMemory -= PAGE_SIZE;
        usedMemory += PAGE_SIZE;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64 pageCount)
{
    for (int t = 0; t < pageCount; t++) {
        LockPage((void*)((uint64)address + (t * PAGE_SIZE))); // Add bytes of single page and convert back to void*
    }
}

void PageFrameAllocator::UnreservePage(void* address)
{
    uint64 index = (uint64)address / PAGE_SIZE;
    if (this->mPageBitmap[index] == false)
        return;

    if (this->mPageBitmap.Set(index, false)) {
        freeMemory += PAGE_SIZE;
        reservedMemory -= PAGE_SIZE;

        if (pageBitmapIndex > index)
            pageBitmapIndex = index;
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64 pageCount)
{
    for (int t = 0; t < pageCount; t++) {
        UnreservePage((void*)((uint64)address + (t * PAGE_SIZE))); // Add bytes of single page and convert back to void*
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64 index = (uint64)address / PAGE_SIZE;
    
    //Check if page is free
    if (this->mPageBitmap[index] == true) 
        return;
    
    if (this->mPageBitmap.Set(index, true)) {
        freeMemory -= PAGE_SIZE;
        reservedMemory += PAGE_SIZE;
    }
}
void PageFrameAllocator::ReservePages(void* address, uint64 pageCount)
{
    for (int t = 0; t < pageCount; t++) {
        ReservePage((void*)((uint64)address + (t * PAGE_SIZE))); // Add bytes of single page and convert back to void*
    }
}

uint64 PageFrameAllocator::GetFreeRAM()
{
    return freeMemory;
}

uint64 PageFrameAllocator::GetUsedRAM()
{
    return usedMemory;
}

uint64 PageFrameAllocator::GetReservedRAM()
{
    return reservedMemory;
}
