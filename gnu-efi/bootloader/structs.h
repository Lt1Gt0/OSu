#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <efi.h>
#include <efilib.h>
#include <elf.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef unsigned long long size_t ;

typedef struct {
	void* 			BaseAddress;
	size_t			BufferSize;
	unsigned int 	Width;
	unsigned int 	Height;
	unsigned int 	PixelsPerScanLine;
} FrameBuffer;

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
	FrameBuffer* 			frameBuffer;
	PSF1_FONT* 				psf1_font;
	EFI_MEMORY_DESCRIPTOR* 	mMap;
	UINTN 					mMapSize;
	UINTN 					mMapDescSize;
	void* 					rsdp;
	void* 					osulogo;
} BootInfo;





#endif // __STRUCTS_H