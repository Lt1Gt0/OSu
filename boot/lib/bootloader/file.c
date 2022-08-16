#include "bootloader/file.h"
#include "bootloader/video.h"
#include "bootloader/string.h"
#include <elf.h>

UINTN GetFileSize(IN EFI_FILE_HANDLE FileHandle)
{
	/*File Information structure */
	UINTN size;
	EFI_FILE_INFO* fileInfo;

	/* Get the size of the file */
	fileInfo = LibFileInfo(FileHandle);
	size = fileInfo->FileSize;
	FreePool(fileInfo);

	return size; 
}

EFI_FILE* LoadFile
(
		IN	EFI_FILE*			Directory, 
		IN	CHAR16*				Path, 
		IN	EFI_HANDLE			ImageHandle, 
		IN	EFI_SYSTEM_TABLE*	SystemTable
)
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&FileSystem); //Get file system where booted from

	//If the directory is null set the directory to the root of the file system
	if (Directory == NULL)
		FileSystem->OpenVolume(FileSystem, &Directory);

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS)
		return NULL;
	
	return LoadedFile;
}

PSF1_FONT* LoadPSF1Font
(
		IN	EFI_FILE*			Directory,
	   	IN	CHAR16*				Path, 
		IN	EFI_HANDLE			ImageHandle, 
		IN	EFI_SYSTEM_TABLE*	SystemTable
)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL)
		return NULL; // File doesnt not exist

	PSF1_HEADER* fontHeader;
	UINTN psf1Headersize = sizeof(PSF1_HEADER);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, psf1Headersize, (VOID**)&fontHeader);
	font->Read(font, &psf1Headersize, fontHeader); // Load header into memory

	// Check for incorrect font format
	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
		return NULL;
	

	UINTN glyphBufferSize = fontHeader->charsize * 256; // Amount of glyph character
	if (fontHeader->mode == 1) //512 glyph mode
		glyphBufferSize = fontHeader->charsize * 512;
	
	// Define glyph buffer and read information into it
	VOID* glyphBuffer;
	
	{
		font->SetPosition(font, psf1Headersize);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (VOID**)&glyphBuffer); // Allocate memory for the glyph buffer
		font->Read(font, &glyphBufferSize, glyphBuffer); // Read data into memory
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (VOID**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;
}

EFI_FILE_HANDLE GetVolume(IN EFI_HANDLE image)
{
	// Instead of using IOVolume this function will just use LibOpenRoot instead

	EFI_LOADED_IMAGE* loadedImage = NULL;
	EFI_GUID lipGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;

	uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGUID, (VOID**)&loadedImage);

	// Get volume handle
	return LibOpenRoot(loadedImage->DeviceHandle);

}

VOID* LoadRawImageFile
(
		IN	EFI_FILE*			Directory, 
		IN	CHAR16*				Path, 
		IN	EFI_HANDLE			ImageHandle, 
		IN	EFI_SYSTEM_TABLE*	SystemTable
)
{
	EFI_FILE_HANDLE ImageVolume = GetVolume(ImageHandle);
	EFI_FILE_HANDLE fileHandle;

	uefi_call_wrapper(ImageVolume->Open, 5, ImageVolume, &fileHandle, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

	UINTN size = GetFileSize(fileHandle);
	VOID* contents = AllocateZeroPool(size);

	uefi_call_wrapper(fileHandle->Read, 3, fileHandle, &size, contents);

	return contents;
}

BOOLEAN ELF_VerifyHeader(IN Elf64_Ehdr* elfHeader)
{
	if (memcmp(&elfHeader->e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0	||
		elfHeader->e_ident[EI_CLASS] != ELFCLASS64					||
		elfHeader->e_ident[EI_DATA] != ELFDATA2LSB					||
		elfHeader->e_type != ET_EXEC								||
		elfHeader->e_machine != EM_X86_64							||
		elfHeader->e_version != EV_CURRENT) {
	
		Print(L"Invalid ELF Header\n\r");
		return FALSE;	
	} else {
		Print(L"ELF Header verified\n\r");
	}

	return TRUE;
}

/*
UINTN ELF_GetTotalFileSize
(
	IN Elf64_Ehdr* elfHeader
)
{

}
*/
