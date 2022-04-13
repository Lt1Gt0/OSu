#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <efiapi.h>
#include "structs.h"

typedef unsigned long long size_t ;

UINTN FileSize(EFI_FILE_HANDLE FileHandle)
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

//Declare and initialize the frame buffer and its protocol for graphics (simple graphics)
FrameBuffer frameBuffer;
FrameBuffer *InitializeGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status; //Check for failures

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if (EFI_ERROR(status)) {
		Print(L"Unable to Locate GOP\n\r");
		return NULL;
	} else { 
		Print(L"GOP Located\n\r"); 
	}

	frameBuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	frameBuffer.BufferSize = gop->Mode->FrameBufferSize;
	frameBuffer.Width = gop->Mode->Info->HorizontalResolution;
	frameBuffer.Height = gop->Mode->Info->VerticalResolution;
	frameBuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &frameBuffer;
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

EFI_FILE *LoadFile(EFI_FILE *Directory, CHAR16 *Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
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

void* LoadRawImageFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE_HANDLE ImageVolume = GetVolume(ImageHandle);
	EFI_FILE_HANDLE fileHandle;

	uefi_call_wrapper(ImageVolume->Open, 5, ImageVolume, &fileHandle, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

	UINTN size = FileSize(fileHandle);
	void* contents = AllocateZeroPool(size);

	uefi_call_wrapper(fileHandle->Read, 3, fileHandle, &size, contents);

	return contents;
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

// Memory Compare function
int memcmp(const void *aptr, const void *bptr, size_t n)
{
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++) {
		if (a[i] < b[i])
			return -1;
			
		else if (a[i] > b[i])
			return 1;
	}

	return 0;
}

UINTN strcmp(CHAR8 *a, CHAR8 *b, UINTN len)
{
	for (UINTN i = 0; i < len; i++) {
		if (*a != *b)
			return 0;

		a++;
		b++;
	}
	return 1;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	InitializeLib(ImageHandle, SystemTable); //Set up the UEFI environment commands
	Print(L"Bootloader Initialized\n\r");

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable); // Load file 
	if (LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable) == NULL) {
		Print(L"Could Not Load Kernel \n\r");
		return EFI_LOAD_ERROR;
	} else { 
		Print(L"Kernel Loaded Successfully\n\r"); 
	}

	Elf64_Ehdr header; // Create ELF Header
	
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL); //Set the file info size to the size of the kernel
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo); // Allocate memory for the ELF Header
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header); // Read amount of bytes in size from the header
	}
	
	void* osulogo = LoadRawImageFile(NULL, L"osulogo.raw", ImageHandle, SystemTable);

	// Because the OSU logo isn't a vital part of booting it won't return an error if not found
	if (!osulogo)
		Print(L"Error Loading OSU Image\n\r");
	else 
		Print(L"Loaded OSU Image successfully\n\r");

	// Check if kernel header is correct
	if (memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT) {

		Print(L"Kernel format is bad\n\r");
		return EFI_ABORTED;
	} else {
		Print(L"Kernel header successfully verified\n\r");
	}

	Elf64_Phdr* phdrs;
	
	{
		Kernel->SetPosition(Kernel, header.e_phoff); // Set offset in bytes when read
		UINTN size = header.e_phnum * header.e_phentsize; // Program header num * Program header entry size
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	// Go through each program header and load their binary information
	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	) {
		switch (phdr->p_type) {
		case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000; // Get memory size and round up
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void *)segment);
				break;
			}
		}
	}
	Print(L"Kernel Loaded\n\r");
	
	// Load the font file into memory
	PSF1_FONT* newFont = LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);
	if (newFont == NULL) {
		Print(L"Font is not valid or not found\n\r");
		return EFI_LOAD_ERROR;
	} else { 
		Print(L"Font Found. Char size = %d\r\n", newFont->psf1_Header->charsize);
	}

	FrameBuffer* newBuffer = InitializeGOP();

	//Print out the base, size, width, height, and pixels per scan line in GOP
	Print(L"Base: 0x%x\n\rSize: 0x%x\n\rWidth: %d\n\rHeight: %d\n\rPixelsPerScanLine: %d\n\r", 
	newBuffer->BaseAddress, 
	newBuffer->BufferSize,
	newBuffer->Width,
	newBuffer->Height,
	newBuffer->PixelsPerScanLine);

	EFI_MEMORY_DESCRIPTOR* Map = NULL; // Pointer to memory descriptor, contains number of pages of the memory section, address of the memory segment, type of memory segment, etc... 
	UINTN MapSize, MapKey; //MapSize - Complete size of the map in bytes. MapKey - Needed for UEFI
	UINTN DescriptorSize; //Size of descriptor entries
	UINT32 DescriptorVersion;
	
	{
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}
  
	EFI_CONFIGURATION_TABLE* configTable = SystemTable->ConfigurationTable;
	void* rsdp = NULL;
	EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;

	for (UINTN index = 0; index < SystemTable->NumberOfTableEntries; index++) {
	    if (CompareGuid(&configTable[index].VendorGuid, &Acpi2TableGuid)) {
	    	if (strcmp((CHAR8*)"RSD PTR ", (CHAR8*)configTable->VendorTable, 8))
    			rsdp = (void*)configTable->VendorTable;
    	}
    	configTable++;
  	}

	void(*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*) ) header.e_entry); // Define an void function pointer at the address of header.e_entry with the attribute provided

	// Define the boot info to pass into the kernel
	BootInfo bootInfo;
	bootInfo.frameBuffer = newBuffer;
	bootInfo.psf1_font = newFont;
	bootInfo.mMap = Map;
	bootInfo.mMapSize = MapSize;
	bootInfo.mMapDescSize = DescriptorSize;
	bootInfo.rsdp = rsdp;
	bootInfo.osulogo = osulogo;

	// Exit the boot services
	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey); //Exit boot services
	
	// Load the kernel in to memeory
	KernelStart(&bootInfo);

	return EFI_SUCCESS; // Exit the UEFI application
}