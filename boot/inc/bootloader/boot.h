#pragma once
#ifndef _BOOT_H
#define _BOOT_H

#include "video.h"

typedef struct {
	FrameBuffer* 			frameBuffer;
	PSF1_FONT* 				psf1_font;
	EFI_MEMORY_DESCRIPTOR* 	mMap;
	UINTN 					mMapSize;
	UINTN 					mMapDescSize;
	void* 					rsdp;
	void* 					osulogo;
} BootInfo;

#endif // _BOOT_H
