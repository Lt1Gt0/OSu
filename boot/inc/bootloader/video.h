#pragma once
#ifndef _BOOT_VIDEO_H
#define _BOOT_VIDEO_H

#include "efi.h"
#include "efilib.h"
#include "common.h"

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
	unsigned char magic[2]; // Identifier bytes
	unsigned char mode; // mode that the psf font is in
	unsigned char charsize; // how large the character are in bytes
} PSF1_HEADER;	

typedef struct {
	PSF1_HEADER* 	psf1_Header;
	void* 			glyphBuffer;
} PSF1_FONT;

typedef struct {
	void* 			BaseAddress;
	size_t			BufferSize;
	unsigned int 	Width;
	unsigned int 	Height;
	unsigned int 	PixelsPerScanLine;
} FrameBuffer;

FrameBuffer* InitializeGOP();

VOID SetForegroundColor(EFI_SYSTEM_TABLE* SystemTable, UINT8 color);
VOID PrintL(IN EFI_SYSTEM_TABLE* st, IN CHAR16* fmt);

/**
 * Because 'c' does not have support for default values, The default colors to return to will
 * be simply a white fg on a black bg until I decide to change that
 */
VOID PrintWithColor(IN EFI_SYSTEM_TABLE* st, IN CHAR16* fmt, IN UINT8 fg);

extern FrameBuffer frameBuffer;

#endif // _BOOT_VIDEO_H
