#pragma once
#ifndef _PAGE_FRAME_ALLOCATOR_H_
#define _PAGE_FRAME_ALLOCATOR_H_

#include <efiMemory.h>
#include <types.h>
#include <bitmap.h>

class PageFrameAllocator
{
    public:
        void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize);
        void FreePage(void* address);
        void FreePages(void* address, uint64 pageCount);
        void LockPage(void* address);
        void LockPages(void* address, uint64 pageCount);
        void* RequestPage();
    
        uint64 GetFreeRAM();
        uint64 GetUsedRAM();
        uint64 GetReservedRAM();

       Bitmap mPageBitmap;

    private:
        /**
         * @brief Initialize a bitmap given a size and a base address
         * 
         * @param bitmapSize Size
         * @param bufferAddress Base address
         */
        void InitBitmap(size_t bitmapSize, void* bufferAddress);
        void ReservePage(void* address);
        void ReservePages(void* address, uint64 pageCount);
        void UnreservePage(void* address);
        void UnreservePages(void* address, uint64 pageCount);
};

extern PageFrameAllocator GlobalAllocator;

#endif // _PAGE_FRAME_ALLOCATOR_H_
