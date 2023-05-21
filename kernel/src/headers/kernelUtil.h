#pragma once
#ifndef _KERNEL_UTIL_H
#define _KERNEL_UTIL_H

#include <ahci/ahci.h>
#include <efiMemory.h>
#include <fonts.h>
#include <framebuffer.h>
#include <types.h>

struct BootInfo {
  FrameBuffer *frameBuffer;
  PSF1_FONT *psf1_font;
  EFI_MEMORY_DESCRIPTOR *mMap;
  uint64 mMapSize;
  uint64 mMapDescSize;
  ACPI::RSDP2 *rsdp;
  void *osulogo;
};

extern "C" uint64 _KERNEL_START_;
extern "C" uint64 _KERNEL_END_;

void InitializeKernel(BootInfo *bootInfo);

#endif // _KERNEL_UTIL_H
