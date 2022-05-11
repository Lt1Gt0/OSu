#pragma once
#ifndef _FRAME_BUFFER_H
#define _FRAME_BUFFER_H

#include <stddef.h>

struct FrameBuffer {
	void* 			BaseAddress;
	size_t 			BufferSize;
	unsigned int 	Width;
	unsigned int 	Height;
	unsigned int 	PixelsPerScanLine;
};

#endif // _FRAME_BUFFER_H