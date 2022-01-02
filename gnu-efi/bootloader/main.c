#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t ;

typedef struct{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} FrameBuffer;

FrameBuffer frameBuffer;
FrameBuffer* InitializeGOP(){
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status; //Check for failures

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if(EFI_ERROR(status)){
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

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem); //Get file system where booted from

	//If the directory is null set the directory to the root of the file system
	if(Directory == NULL){
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS) return NULL;
	return LoadedFile;
}

// Memory Compare function
int memcmp(const void* aptr, const void* bptr, size_t n){
	const unsigned char* a = aptr, *b = bptr;
	for(size_t i = 0; i < n; i++){
		if(a[i] < b[i]) return -1;
		else if(a[i] > b[i]) return 1;
	}
	return 0;
}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable); //Set up the UEFI environment commands
	Print(L"Bootloader Initialized\n\r");

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable); // Load file 
	if (LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable) == NULL){
		Print(L"Could Not Load Kernel \n\r");
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

	// Check if kernel header is correct
	if(
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print(L"Kernel format is bad\n\r");
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
	for(
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	)
	{
		switch(phdr->p_type){
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000; //Get memory size and round up
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);
				break;
			}
		}
	}
	Print(L"Kernel Loaded\n\r");

	void(*KernelStart)(FrameBuffer*) = ((__attribute__((sysv_abi)) void (*)(FrameBuffer*) ) header.e_entry); // Define an void function pointer at the address of header.e_entry with the attribute provided
	FrameBuffer* newBuffer = InitializeGOP();

	Print(L"Base: 0x%x\n\rSize: 0x%x\n\rWidth: %d\n\rHeight: %d\n\rPixelsPerScanLine: %d\n\r\n\r", 
	newBuffer->BaseAddress, 
	newBuffer->BufferSize,
	newBuffer->Width,
	newBuffer->Height,
	newBuffer->PixelsPerScanLine);

	// unsigned int y = 50;
	// unsigned int BPP = 4;// Bytes per pixel

	// for(unsigned int x = 0; x < newBuffer->Width/2 * BPP; x+=BPP){
	// 	*(unsigned int*)(x + (y * newBuffer->PixelsPerScanLine * BPP) + newBuffer->BaseAddress ) = 0x00ff00ff;// Calculate address
    // }

	KernelStart(newBuffer);

	return EFI_SUCCESS; // Exit the UEFI application
}
