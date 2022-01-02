typedef unsigned long long size_t; // Make size_t a macro in the future

typedef struct{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} FrameBuffer;

typedef struct{
	unsigned char magic[2]; // Identifier bytes
	unsigned char mode; // mode that the psf font is in
	unsigned char charsize; // how large the character are in bytes
} PSF1_HEADER;

typedef struct{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

typedef struct{
    unsigned int X;
    unsigned int Y;
} Point;

void putChar(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font, unsigned int color, char chr, unsigned int xOff, unsigned int yOff){
    unsigned int* pixelPtr = (unsigned int*)frameBuffer->BaseAddress;
    char* fontPtr = (char*)psf1_font->glyphBuffer + (chr * psf1_font->psf1_Header->charsize); // Get base address of the glyph buffer + character * character size
    for(unsigned long y = yOff; y < yOff + 16; y++){
        for(unsigned long x = xOff; x < xOff + 8; x++){
            if((*fontPtr & 0b10000000 >> (x - xOff)) > 0){ // Bitshift single bit to the right by (x - xOff) to select bit from bitmap
                *(unsigned int*)(pixelPtr + x + (y * frameBuffer->PixelsPerScanLine)) = color;
            }
        }
        fontPtr++;
    } 
}

Point CursorPosition;
void Print(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font, unsigned int color, char* str){
    char* chr = str;
    while(*chr != 0){
        putChar(frameBuffer, psf1_font, color, *chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;

        if(CursorPosition.X + 8 > frameBuffer->Width){
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }

        chr++;
    }
}
extern "C" void _start(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font){
    CursorPosition.X = 0;
    CursorPosition.Y = 0;

    Print(frameBuffer, psf1_font, 0xffffffff, "Hello Kernel");

    return;
}