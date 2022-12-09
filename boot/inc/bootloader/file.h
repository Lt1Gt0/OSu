#pragma once
#ifndef _BOOT_FILE_H
#define _BOOT_FILE_H

#include "efi.h"
#include "efilib.h"
#include "elf.h"
#include "efiapi.h"
#include "efibind.h"
#include "efidef.h"

#include "common.h"
#include "video.h"

UINTN GetFileSize
(
	IN EFI_FILE_HANDLE FileHandle
);

EFI_FILE* LoadFile
(
	IN	EFI_FILE*			Directory, 
	IN	CHAR16*				Path, 
	IN	EFI_HANDLE			ImageHandle, 
	IN	EFI_SYSTEM_TABLE*	SystemTable
);

PSF1_FONT* LoadPSF1Font
(
	IN	EFI_FILE*			Directory,
	IN	CHAR16*				Path, 
	IN	EFI_HANDLE			ImageHandle, 
	IN	EFI_SYSTEM_TABLE*	SystemTable
);

EFI_FILE_HANDLE GetVolume
(
	IN EFI_HANDLE image
);

VOID* LoadRawImageFile
(
	IN	EFI_FILE*			Directory, 
	IN	CHAR16*				Path, 
	IN	EFI_HANDLE			ImageHandle, 
	IN	EFI_SYSTEM_TABLE*	SystemTable
);

/* ELF File operations */

VOID ELF_VerifyHeader
(
	IN Elf64_Ehdr* elfHeader
);

VOID ELF_Load64
(
    IN EFI_FILE* file, 
    IN EFI_SYSTEM_TABLE* SystemTable, 
    IN OUT Elf64_Ehdr* elfHeader
);

UINTN ELF_GetTotalFileSize
(
	IN Elf64_Ehdr* elfHeader
);

//EFI_STATUS ELF_Load
//(
	//IN	VOID* buffer,
	//IN	UINT64 physicalAddr
//);

#endif // _BOOT_FILE_H
