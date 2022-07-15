#pragma once
#ifndef _BOOT_FILE_H
#define _BOOT_FILE_H

#include "efi.h"
#include "efilib.h"
#include "elf.h"
#include "efiapi.h"

#include "common.h"
#include "video.h"

UINTN GetFileSize(EFI_FILE_HANDLE FileHandle);
EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

EFI_FILE_HANDLE GetVolume(EFI_HANDLE image);
void* LoadRawImageFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

#endif // _BOOT_FILE_H
