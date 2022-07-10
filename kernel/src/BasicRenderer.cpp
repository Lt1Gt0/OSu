#include <BasicRenderer.h>
#include <colors.h>

BasicRenderer GlobalRenderer(nullptr, nullptr);
BasicRenderer::BasicRenderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* psf1_Font)
{
    TargetFrameBuffer = targetFrameBuffer;
    PSF1_Font = psf1_Font;
    Color = Colors::TTY::WHITE; // Default frambuffer foreground color set to white
    CursorPosition = {0, 0}; // Default framebuffer cursor position set to x = 0, y = 0
}

void BasicRenderer::Clear()
{
    uint64 fbBase = (uint64)TargetFrameBuffer->BaseAddress;
    uint64 bytesPerScanLine = TargetFrameBuffer->PixelsPerScanLine * 4; // Bytes in one row
    uint64 fbHeight = TargetFrameBuffer->Height;
    uint64 fbSize = TargetFrameBuffer->BufferSize;

    /* Go through each row in the frame buffer and set each value to the specified color */
    for (int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++) {
        uint64 pixPtrBase = fbBase + (bytesPerScanLine * verticalScanLine); // point to the first pixel in row
        for (uint32* pixPtr = (uint32*)pixPtrBase; pixPtr < (uint32*)(pixPtrBase + bytesPerScanLine); pixPtr++) {
            *pixPtr = ClearColor; // Set the color
        }
    }
}

void BasicRenderer::ClearChar()
{
    if (CursorPosition.X == 0) {
        CursorPosition.X = TargetFrameBuffer->Width;
        CursorPosition.Y -= 16;

        if(CursorPosition.Y > 0)
            CursorPosition.Y = 0;
    }
    
    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

    unsigned int* pixelPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++) { // 16 is added as the height of a character (glyph)
        for (unsigned long x = xOff - 8; x < xOff; x++) { // 8 is added as the width of a character (glyph)
                *(unsigned int*)(pixelPtr + x + (y * TargetFrameBuffer->PixelsPerScanLine)) = ClearColor;
        }  
    }
    
    CursorPosition.X -= 8;
    if (CursorPosition.X < 0) {
        CursorPosition.X = TargetFrameBuffer->Width;
        CursorPosition.Y -= 16;

        if(CursorPosition.Y < 0)
            CursorPosition.Y = 0;
    }
}

void BasicRenderer::Next()
{
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
}

void BasicRenderer::Print(const char* str)
{
    // Todo, exclude/include special character printing if desired

    char* chr = (char*)str;
    while (*chr != 0) {


        PutChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X += 8;

        if (CursorPosition.X + 8 > TargetFrameBuffer->Width) {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }

        chr++;
    }

    // Color = Colors::TTY::WHITE;
}

void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff)
{
    // Incase a font file has a glyph for a special character that doesn't
    // need to be printed, just do whatever is needed for the special char
    // and don't print it
    switch (chr) {
        case '\n':
        {
            GlobalRenderer.Next();
            return;
        }
    }

    unsigned int* pixelPtr = (unsigned int*)TargetFrameBuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize); // Get base address of the glyph buffer + character * character size
    for (unsigned long y = yOff; y < yOff + 16; y++) { // 16 is added as the height of a character (glyph)
        for (unsigned long x = xOff; x < xOff + 8; x++) { // 8 is added as the width of a character (glyph)
            if ((*fontPtr & 0b10000000 >> (x - xOff)) > 0) { // Bitshift single bit to the right by (x - xOff) to select bit from bitmap
                *(unsigned int*)(pixelPtr + x + (y * TargetFrameBuffer->PixelsPerScanLine)) = Color;
            }
        }

        fontPtr++;
    }
}

void BasicRenderer::PutChar(char chr)
{
    PutChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    if (CursorPosition.X + 8 > TargetFrameBuffer->Width) {
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }
}

uint32 BasicRenderer::GetPix(uint32 x, uint32 y)
{
    return *(uint32*)((uint64)TargetFrameBuffer->BaseAddress + (x*4) + (y * TargetFrameBuffer->PixelsPerScanLine * 4));
}

void BasicRenderer::PutPix(uint32 x, uint32 y, uint32 color)
{
    *(uint32*)((uint64)TargetFrameBuffer->BaseAddress + (x*4) + (y * TargetFrameBuffer->PixelsPerScanLine * 4)) = color;
}

void BasicRenderer::ClearMouseCursor(uint8 *mouseCursor, Point position)
{
    if (!MouseDrawn)
        return;

    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFrameBuffer->Width - position.X;
    int differenceY = TargetFrameBuffer->Height - position.Y;

    if (differenceX < 16)
        xMax = differenceX;
    
    if (differenceY < 16)
        yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;

            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                if (GetPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y * 16]) 
                    PutPix(position.X + x, position.Y + y, MouseCursorBuffer[x + y * 16]); 
            }
        }
    }
}

void BasicRenderer::DrawOverlayMouseCursor(uint8 *mouseCursor, Point position, uint32 color)
{
    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFrameBuffer->Width - position.X;
    int differenceY = TargetFrameBuffer->Height - position.Y;

    if (differenceX < 16)
        xMax = differenceX;
    
    if (differenceY < 16)
        yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
        
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                MouseCursorBuffer[x + y * 16] = GetPix(position.X + x, position.Y + y);
                PutPix(position.X + x, position.Y + y, color);
                MouseCursorBufferAfter[x + y * 16] = GetPix(position.X + x, position.Y + y);
            }
        }
    }
    
    MouseDrawn = true;
}