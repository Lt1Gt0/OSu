#pragma once
#ifndef _KERNEL_UTIL_H
#define _KERNEL_UTIL_H

#include <stdint.h>
#include <FrameBuffer.h>
#include <SimpleFonts.h>
#include <ahci/ahci.h>
#include <efiMemory.h>
#include <paging/PageTableManager.h>

struct BootInfo {
	FrameBuffer* 			frameBuffer;
	PSF1_FONT* 				psf1_font;
	EFI_MEMORY_DESCRIPTOR* 	mMap;
	uint64_t 				mMapSize;
	uint64_t 				mMapDescSize;
  	ACPI::RSDP2* 			rsdp;
	void* 					osulogo;
};

struct KernelInfo {
    PageTableManager* pageTableManager;
};

extern "C" uint64_t _KernelStart;
extern "C" uint64_t _KernelEnd;

KernelInfo InitializeKernel(BootInfo* bootInfo);

#endif // _KERNEL_UTIL_H