#include "bootloader/file.h"
#include "bootloader/video.h"

UINTN GetFileSize(EFI_FILE_HANDLE FileHandle)
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

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem); //Get file system where booted from

	//If the directory is null set the directory to the root of the file system
	if (Directory == NULL)
		FileSystem->OpenVolume(FileSystem, &Directory);

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS)
		return NULL;
	
	return LoadedFile;
}

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL)
		return NULL; // File doesnt not exist

	PSF1_HEADER* fontHeader;
	UINTN psf1Headersize = sizeof(PSF1_HEADER);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, psf1Headersize, (void**)&fontHeader);
	font->Read(font, &psf1Headersize, fontHeader); // Load header into memory

	// Check for incorrect font format
	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
		return NULL;
	

	UINTN glyphBufferSize = fontHeader->charsize * 256; // Amount of glyph character
	if (fontHeader->mode == 1) //512 glyph mode
		glyphBufferSize = fontHeader->charsize * 512;
	
	// Define glyph buffer and read information into it
	void* glyphBuffer;
	
	{
		font->SetPosition(font, psf1Headersize);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer); // Allocate memory for the glyph buffer
		font->Read(font, &glyphBufferSize, glyphBuffer); // Read data into memory
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;
}

EFI_FILE_HANDLE GetVolume(EFI_HANDLE image)
{
	// Instead of using IOVolume this function will just use LibOpenRoot instead

	EFI_LOADED_IMAGE* loadedImage = NULL;
	EFI_GUID lipGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;

	uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGUID, (void**)&loadedImage);

	// Get volume handle
	return LibOpenRoot(loadedImage->DeviceHandle);

}

void* LoadRawImageFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE_HANDLE ImageVolume = GetVolume(ImageHandle);
	EFI_FILE_HANDLE fileHandle;

	uefi_call_wrapper(ImageVolume->Open, 5, ImageVolume, &fileHandle, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

	UINTN size = GetFileSize(fileHandle);
	void* contents = AllocateZeroPool(size);

	uefi_call_wrapper(fileHandle->Read, 3, fileHandle, &size, contents);

	return contents;
}
