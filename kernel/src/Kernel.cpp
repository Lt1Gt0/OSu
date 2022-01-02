typedef unsigned long long size_t; // Make size_t a macro in the future

typedef struct{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} FrameBuffer;

extern "C" void _start(FrameBuffer* frameBuffer){
	unsigned int y = 50;
	unsigned int BPP = 4;// Bytes per pixel

	for(unsigned int x = 0; x < frameBuffer->Width/2 * BPP; x+=BPP){
    *(unsigned int*)(x + (y * frameBuffer->PixelsPerScanLine * BPP) + frameBuffer->BaseAddress) = 0x00ff00ff;// Calculate address (DONT KNOW WHY THROWS AN ERROR, IT WORKS JUST FINE, NEED TO FIX THIS)
    }
    
    return;
}