#pragma once
#ifndef _KERNEL_UTIL_H
#define _KERNEL_UTIL_H

#include <types.h>
#include <framebuffer.h>
#include <fonts.h>
#include <ahci/ahci.h>
#include <efiMemory.h>

struct BootInfo {
	FrameBuffer* 			frameBuffer;
	PSF1_FONT* 				psf1_font;
	EFI_MEMORY_DESCRIPTOR* 	mMap;
	uint64 					mMapSize;
	uint64 					mMapDescSize;
  	ACPI::RSDP2* 			rsdp;
	void* 					osulogo;
};

extern "C" uint64 _KernelStart;
extern "C" uint64 _KernelEnd;

void InitializeKernel(BootInfo* bootInfo);

#endif // _KERNEL_UTIL_H
