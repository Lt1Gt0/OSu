#include "PageFrameAllocator.h"

uint64_t freeMemory; // Free memory of the entire system
uint64_t reservedMemory; // Memory reserved by ACPI tables, etc
uint64_t usedMemory; // self explanitory
bool Initialized = false;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize){
    if(Initialized) return;
    Initialized = true;

    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    uint64_t mMapEntries = mMapSize / mMapDescSize;
    for(int i = 0; i < mMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));

        if(desc->type == 7){ //type = EfiConventionalMemory
            if(desc->numPages * 4096 > largestFreeMemSegSize){
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->numPages * 4096;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescSize);
    freeMemory = memorySize; // All current system memory is free

    // Calculate needed size for the bitmap
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    // Initialize bitmap
    InitBitmap(bitmapSize, largestFreeMemSeg);

    // Lock pages of bitmap and reserve pages of unusable/reserved memory
    LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);

    for(int i = 0; i < mMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if(desc->type != 7){ // not efiConventionalMemory
            ReservePages(desc->physAddr, desc->numPages);
        }        
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress){
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*)bufferAddress;
    for(int i = 0; i < bitmapSize; i++){
        *(uint8_t*)(PageBitmap.Buffer + i)  = 0;
    }
}

void* PageFrameAllocator::RequestPage(){
    //Subject to future optimization
    for(uint64_t index = 0; index < PageBitmap.Size * 8; index++){
        if(PageBitmap[index] == true) continue;
        LockPage((void*)(index * 4096));
        return (void*)(index*4096); //Memory address of new free page frame
    }

    return NULL; //Page frame swap to file
}

void PageFrameAllocator::FreePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    //Check if page is free
    if(PageBitmap[index] == false) return;
    PageBitmap.Set(index, false); // Make free
    freeMemory += 4096;
    usedMemory += 4096;
}
void PageFrameAllocator::FreePages(void* address, uint64_t pageCount){
    for(int t = 0; t < pageCount; t++){
        FreePage((void*)((uint64_t)address + (t * 4096))); // Add bytes of single page and convert back to void*
    }
}

void PageFrameAllocator::LockPage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    //Check if page is Locked
    if(PageBitmap[index] == true) return;
    PageBitmap.Set(index, true); // Make locked
    freeMemory -= 4096;
    usedMemory += 4096;
}
void PageFrameAllocator::LockPages(void* address, uint64_t pageCount){
    for(int t = 0; t < pageCount; t++){
        LockPage((void*)((uint64_t)address + (t * 4096))); // Add bytes of single page and convert back to void*
    }
}

void PageFrameAllocator::UnreservePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if(PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}
void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount){
    for(int t = 0; t < pageCount; t++){
        UnreservePage((void*)((uint64_t)address + (t * 4096))); // Add bytes of single page and convert back to void*
    }
}

void PageFrameAllocator::ReservePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    //Check if page is free
    if(PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}
void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount){
    for(int t = 0; t < pageCount; t++){
        ReservePage((void*)((uint64_t)address + (t * 4096))); // Add bytes of single page and convert back to void*
    }
}

uint64_t PageFrameAllocator::GetFreeRAM(){
    return freeMemory;
}
uint64_t PageFrameAllocator::GetUsedRAM(){
    return usedMemory;
}
uint64_t PageFrameAllocator::GetReservedRAM(){
    return reservedMemory;
}