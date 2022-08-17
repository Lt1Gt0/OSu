#pragma once
#ifndef _FRAME_BUFFER_H
#define _FRAME_BUFFER_H

#include <types.h>

struct FrameBuffer {
	void*	BaseAddress;
	size_t	BufferSize;
	uint32	Width;
	uint32 	Height;
	uint32 	PixelsPerScanLine;
};

#endif // _FRAME_BUFFER_H
