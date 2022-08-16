#include "bootloader/video.h"

FrameBuffer frameBuffer;
FrameBuffer* InitializeGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status; //Check for failures

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (VOID**)&gop);
	if (EFI_ERROR(status)) {
		Print(L"Unable to Locate GOP\n\r");
		return NULL;
	} else { 
		Print(L"GOP Located\n\r"); 
	}

	frameBuffer.BaseAddress = (VOID*)gop->Mode->FrameBufferBase;
	frameBuffer.BufferSize = gop->Mode->FrameBufferSize;
	frameBuffer.Width = gop->Mode->Info->HorizontalResolution;
	frameBuffer.Height = gop->Mode->Info->VerticalResolution;
	frameBuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &frameBuffer;
}
