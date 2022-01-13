#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include "Bitmap.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/paging.h"
#include "paging/PageTableManager.h"

struct BootInfo {
	FrameBuffer* frameBuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern "C" uint64_t _KernelStart;
extern "C" uint64_t _KernelEnd;

extern "C" void _start(BootInfo* bootInfo){
    BasicRenderer newRenderer = BasicRenderer(bootInfo->frameBuffer, bootInfo->psf1_font); // Initialize a renderer
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize; // get the total map entries by dividing the size of the map by the descriptor size

    GlobalAllocator = PageFrameAllocator(); // Declare the global allocator to an instance of a PageFrameAllocator
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    // Get the kernel size 
    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 0x1000 + 1;
    
    // Lock the pages that correspond to kernel to prevent corrupting the kernel's code 
    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    //Set the page table
    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(PML4);

    for(uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }  

    // Calculate the frame buffer size
    uint64_t fbBase = (uint64_t)bootInfo->frameBuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->frameBuffer->BufferSize + 0x1000;

    for(uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t); //Map the frame buffer pages to virtual memory
    }

    asm("mov %0, %%cr3" : : "r" (PML4));
    
    /* Test to see if the PTM works as intended */
    //newRenderer.Print("New Page map time");
    //Map an overly large physical memory address to a valid virtual address
    pageTableManager.MapMemory((void*)0x600000000, (void*)0x80000);
    uint64_t* test = (uint64_t*)0x600000000;
    *test = 26;

    newRenderer.Print(to_string(*test)); // Print the test value for test (26)

    return;
}

