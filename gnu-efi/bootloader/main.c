#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <efiapi.h>

#include "structs.h"
#include "functions.h"

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