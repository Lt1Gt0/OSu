#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* psf1_Font)
{
    TargetFrameBuffer = targetFrameBuffer;
    PSF1_Font = psf1_Font;
    Color = 0xffffffff; // Default frambuffer foreground color set to white
    CursorPosition = {0, 0}; // Default framebuffer cursor position set to x = 0, y = 0
}
void BasicRenderer::Print(const char* str)
{
    char* chr = (char*)str;
    while(*chr != 0){
        PutChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;

        if(CursorPosition.X + 8 > TargetFrameBuffer->Width){
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }

        chr++;
    }
}

void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff){
    unsigned int* pixelPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize); // Get base address of the glyph buffer + character * character size
    for(unsigned long y = yOff; y < yOff + 16; y++){ // 16 is added as the height of a character (glyph)
        for(unsigned long x = xOff; x < xOff + 8; x++){ // 8 is added as the width of a character (glyph)
            if((*fontPtr & 0b10000000 >> (x - xOff)) > 0){ // Bitshift single bit to the right by (x - xOff) to select bit from bitmap
                *(unsigned int*)(pixelPtr + x + (y * TargetFrameBuffer->PixelsPerScanLine)) = Color;
            }
        }
        fontPtr++;
    } 
}