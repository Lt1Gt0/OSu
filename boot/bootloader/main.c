#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <efiapi.h>

#include <bootloader/boot.h>
#include <bootloader/video.h>
#include <bootloader/string.h>
#include <bootloader/file.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable); //Set up the UEFI environment commands
    PrintWithColor(SystemTable, L"Bootloader Initialized\n\r", EFI_GREEN);

	// Attempt to load the kernel file
	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable) == NULL) {
        PrintWithColor(SystemTable, L"Unable to load kernel", EFI_RED);
		return -1; // Not the correct way to return from errors
	} else { 
        PrintWithColor(SystemTable, L"Kernel loaded successfully\n\r", EFI_GREEN);
	}

    Elf64_Ehdr elfHeader;

    {
        UINTN FileInfoSize;
        EFI_FILE_INFO* FileInfo;
        Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL); //Set the file info size to the size of the kernel
        SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo); // Allocate memory for the ELF Header
        Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);
        UINTN size = sizeof(elfHeader);
        Kernel->Read(Kernel, &size, &elfHeader); // Read amount of bytes in size from the elfHeader
    }

    ELF_VerifyHeader(&elfHeader);
    Elf64_Phdr* phdrs;

    {
        Kernel->SetPosition(Kernel, elfHeader.e_phoff); // Set offset in bytes when read
        UINTN size = elfHeader.e_phnum * elfHeader.e_phentsize; // Program elfHeader num * Program elfHeader entry size
        SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
        Kernel->Read(Kernel, &size, phdrs);
    }

	// Go through each program elfHeader and load their binary information
	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + elfHeader.e_phnum * elfHeader.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + elfHeader.e_phentsize)
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

    Print(L"ELF File loaded\n\r");
    PrintWithColor(SystemTable, L"Kernel Loaded", EFI_GREEN);
	
    /*PrintWithColor(SystemTable, L"Kernel loaded\n\r", EFI_GREEN);*/
    VOID* osulogo = LoadRawImageFile(NULL, L"osulogo.raw", ImageHandle, SystemTable);

    /*Because the OSU logo isn't a vital part of booting it won't return an error if not found*/
    if (!osulogo) {
        PrintWithColor(SystemTable, L"Unable to load OSU logo\n\r", EFI_RED);
    } else {
        PrintWithColor(SystemTable, L"Loaded OSU logo\n\r", EFI_GREEN);
    }

	// Load the font file into memory
	PSF1_FONT* newFont = LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);

	if (newFont == NULL) {
        PrintWithColor(SystemTable, L"Font is not valid or not found\n\r", EFI_RED);
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
	UINTN MapSize = 0; //MapSize - Complete size of the map in bytes. MapKey - Needed for UEFI
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0; //Size of descriptor entries
	UINT32 DescriptorVersion = 0;
	
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

	void(*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*)) elfHeader.e_entry);

	// store boot info
	BootInfo bootInfo;
	bootInfo.frameBuffer = newBuffer;
	bootInfo.psf1_font = newFont;
	bootInfo.mMap = Map;
	bootInfo.mMapSize = MapSize;
	bootInfo.mMapDescSize = DescriptorSize;
	bootInfo.rsdp = rsdp;
    bootInfo.osulogo = osulogo;

	// Exit the boot services
	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	
	// Load the kernel in to memeory
	KernelStart(&bootInfo);
	return EFI_SUCCESS; // Exit the UEFI application
}
