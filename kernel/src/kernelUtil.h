#pragma once

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
#include "userinput/mouse.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "IO.h"

struct BootInfo {
	FrameBuffer* frameBuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern "C" uint64_t _KernelStart;
extern "C" uint64_t _KernelEnd;

struct KernelInfo{
    PageTableManager* pageTableManager;
};

KernelInfo InitializeKernel(BootInfo* bootInfo);
