#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* psf1_Font)
{
    TargetFrameBuffer = targetFrameBuffer;
    PSF1_Font = psf1_Font;
    Color = 0xffffffff; // Default frambuffer foreground color set to white
    CursorPosition = {0, 0}; // Default framebuffer cursor position set to x = 0, y = 0
}

void BasicRenderer::Clear(){
    uint64_t fbBase = (uint64_t)TargetFrameBuffer->BaseAddress;
    uint64_t bytesPerScanLine = TargetFrameBuffer->PixelsPerScanLine * 4; // Bytes in one row
    uint64_t fbHeight = TargetFrameBuffer->Height;
    uint64_t fbSize = TargetFrameBuffer->BufferSize;

    /* Go through each row in the frame buffer and set each value to the specified color */
    for(int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++){
        uint64_t pixPtrBase = fbBase + (bytesPerScanLine * verticalScanLine); // point to the first pixel in row
        for(uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanLine); pixPtr++){
            *pixPtr = ClearColor; // Set the color
        }
    }
}

void BasicRenderer::ClearChar(){
    if(CursorPosition.X == 0){
        CursorPosition.X = TargetFrameBuffer->Width;
        CursorPosition.Y -= 16;
        if(CursorPosition.Y > 0) CursorPosition.Y = 0;
    }
    
    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

    unsigned int* pixelPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
    for(unsigned long y = yOff; y < yOff + 16; y++){ // 16 is added as the height of a character (glyph)
        for(unsigned long x = xOff - 8; x < xOff; x++){ // 8 is added as the width of a character (glyph)
                *(unsigned int*)(pixelPtr + x + (y * TargetFrameBuffer->PixelsPerScanLine)) = ClearColor;
        }  
    }
    
    CursorPosition.X -= 8;
    if(CursorPosition.X < 0){
        CursorPosition.X = TargetFrameBuffer->Width;
        CursorPosition.Y -= 16;
        if(CursorPosition.Y < 0) CursorPosition.Y = 0;
    }
}

void BasicRenderer::Next(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
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

void BasicRenderer::PutChar(char chr){
    PutChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    if(CursorPosition.X + 8 > TargetFrameBuffer->Width){
      CursorPosition.X = 0;
      CursorPosition.Y += 16;
    }
}

uint32_t BasicRenderer::GetPix(uint32_t x, uint32_t y){
    return *(uint32_t*)((uint64_t)TargetFrameBuffer->BaseAddress + (x*4) + (y * TargetFrameBuffer->PixelsPerScanLine * 4));
}

void BasicRenderer::PutPix(uint32_t x, uint32_t y, uint32_t color){
    *(uint32_t*)((uint64_t)TargetFrameBuffer->BaseAddress + (x*4) + (y * TargetFrameBuffer->PixelsPerScanLine * 4)) = color;
}

void BasicRenderer::ClearMouseCursor(uint8_t* mouseCursor, Point position){
    if(!MouseDrawn) return;

    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFrameBuffer->Width - position.X;
    int differenceY = TargetFrameBuffer->Height - position.Y;

    if(differenceX < 16) xMax = differenceX;
    if(differenceY < 16) yMax = differenceY;

    for(int y = 0; y < yMax; y++){
        for(int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;
            if((mouseCursor[byte] & (0b10000000 >> (x % 8)))){
                if(GetPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y * 16]){
                  PutPix(position.X + x, position.Y + y, MouseCursorBuffer[x + y * 16]);
                }
            }
        }
    }
}

void BasicRenderer::DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t color){
    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFrameBuffer->Width - position.X;
    int differenceY = TargetFrameBuffer->Height - position.Y;

    if(differenceX < 16) xMax = differenceX;
    if(differenceY < 16) yMax = differenceY;

    for(int y = 0; y < yMax; y++){
      for(int x = 0; x < xMax; x++){
        int bit = y * 16 + x;
        int byte = bit / 8;
        if((mouseCursor[byte] & (0b10000000 >> (x % 8)))){
          MouseCursorBuffer[x + y * 16] = GetPix(position.X + x, position.Y + y);
          PutPix(position.X + x, position.Y + y, color);
          MouseCursorBufferAfter[x + y * 16] = GetPix(position.X + x, position.Y + y);
        }
      }
    }
    MouseDrawn = true;
}
