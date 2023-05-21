#include <renderer.h>
#include <colors.h>

Renderer GlobalRenderer(nullptr, nullptr);
Renderer::Renderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* psf1_Font)
{
    this->mTargetFrameBuffer = targetFrameBuffer;
    this->mPSF1Font = psf1_Font;
    this->mColor = Colors::TTY::WHITE; // Default frambuffer foreground color set to white
    this->mCursorPosition = {0, 0}; // Default framebuffer cursor position set to x = 0, y = 0
}

void Renderer::Clear()
{
    uint64 fbBase = (uint64)this->mTargetFrameBuffer->BaseAddress;
    uint64 bytesPerScanLine = this->mTargetFrameBuffer->PixelsPerScanLine * 4; // Bytes in one row
    uint64 fbHeight = this->mTargetFrameBuffer->Height;
    uint64 fbSize = this->mTargetFrameBuffer->BufferSize;

    /* Go through each row in the frame buffer and set each value to the specified color */
    for (int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++) {
        uint64 pixPtrBase = fbBase + (bytesPerScanLine * verticalScanLine); // point to the first pixel in row
        for (uint32* pixPtr = (uint32*)pixPtrBase; pixPtr < (uint32*)(pixPtrBase + bytesPerScanLine); pixPtr++) {
            *pixPtr = this->mClearColor; // Set the color
        }
    }
}

void Renderer::ClearChar()
{
    if (this->mCursorPosition.X == 0) {
        this->mCursorPosition.X = this->mTargetFrameBuffer->Width;
        this->mCursorPosition.Y -= 16;

        if(this->mCursorPosition.Y > 0)
            this->mCursorPosition.Y = 0;
    }
    
    uint32 xOff = this->mCursorPosition.X;
    uint32 yOff = this->mCursorPosition.Y;

    uint32* pixelPtr = (uint32*)this->mTargetFrameBuffer->BaseAddress;
    for (size_t y = yOff; y < yOff + 16; y++) { // 16 is added as the height of a character (glyph)
        for (size_t x = xOff - 8; x < xOff; x++) { // 8 is added as the width of a character (glyph)
                *(uint32*)(pixelPtr + x + (y * this->mTargetFrameBuffer->PixelsPerScanLine)) = this->mClearColor;
        }  
    }
    
    this->mCursorPosition.X -= 8;
    if (this->mCursorPosition.X < 0) {
        this->mCursorPosition.X = this->mTargetFrameBuffer->Width;
        this->mCursorPosition.Y -= 16;

        if(this->mCursorPosition.Y < 0)
            this->mCursorPosition.Y = 0;
    }
}

void Renderer::Next()
{
    this->mCursorPosition.X = 0;
    this->mCursorPosition.Y += 16;
}

void Renderer::Print(const char* str)
{
    char* chr = (char*)str;
    while (*chr != 0) {

        PutChar(*chr, this->mCursorPosition.X, this->mCursorPosition.Y);
        this->mCursorPosition.X += 8;

        if (this->mCursorPosition.X + 8 > this->mTargetFrameBuffer->Width) {
            this->mCursorPosition.X = 0;
            this->mCursorPosition.Y += 16;
        }

        chr++;
    }
}

void Renderer::PutChar(char chr, uint32 xOff, uint32 yOff)
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

    uint32* pixelPtr = (uint32*)this->mTargetFrameBuffer->BaseAddress;

    // Get base address of the glyph buffer + character * character size
    char* fontPtr = (char*)this->mPSF1Font->glyphBuffer + (chr * this->mPSF1Font->psf1Header->charsize); 

    // 16 is added as the height of a character (glyph)
    for (size_t y = yOff; y < yOff + 16; y++) { 
        // 8 is added as the width of a character (glyph)
        for (size_t x = xOff; x < xOff + 8; x++) { 
            // Bitshift single bit to the right by (x - xOff) to select bit from bitmap
            if ((*fontPtr & 0b10000000 >> (x - xOff)) > 0) { 
                *(uint32*)(pixelPtr + x + (y * this->mTargetFrameBuffer->PixelsPerScanLine)) = this->mColor;
            }
        }

        fontPtr++;
    }
}

void Renderer::PutChar(char chr)
{
    PutChar(chr, this->mCursorPosition.X, this->mCursorPosition.Y);
    this->mCursorPosition.X += 8;
    if (this->mCursorPosition.X + 8 > this->mTargetFrameBuffer->Width) {
        this->mCursorPosition.X = 0;
        this->mCursorPosition.Y += 16;
    }
}

uint32 Renderer::GetPix(uint32 x, uint32 y)
{
    return *(uint32*)((uint64)this->mTargetFrameBuffer->BaseAddress + (x*4) + (y * this->mTargetFrameBuffer->PixelsPerScanLine * 4));
}

void Renderer::PutPix(uint32 x, uint32 y, uint32 color)
{
    *(uint32*)((uint64)this->mTargetFrameBuffer->BaseAddress + (x*4) + (y * this->mTargetFrameBuffer->PixelsPerScanLine * 4)) = color;
}

void Renderer::ClearMouseCursor(uint8 *mouseCursor, Point position)
{
    if (!this->mMouseDrawn)
        return;

    int xMax = 16;
    int yMax = 16;
    int differenceX = this->mTargetFrameBuffer->Width - position.X;
    int differenceY = this->mTargetFrameBuffer->Height - position.Y;

    if (differenceX < 16)
        xMax = differenceX;
    
    if (differenceY < 16)
        yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;

            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                if (GetPix(position.X + x, position.Y + y) == this->mMouseCursorBufferAfter[x + y * 16]) 
                    PutPix(position.X + x, position.Y + y, this->mMouseCursorBuffer[x + y * 16]); 
            }
        }
    }
}

void Renderer::DrawOverlayMouseCursor(uint8 *mouseCursor, Point position, uint32 color)
{
    int xMax = 16;
    int yMax = 16;
    int differenceX = this->mTargetFrameBuffer->Width - position.X;
    int differenceY = this->mTargetFrameBuffer->Height - position.Y;

    if (differenceX < 16)
        xMax = differenceX;
    
    if (differenceY < 16)
        yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
        
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                this->mMouseCursorBuffer[x + y * 16] = GetPix(position.X + x, position.Y + y);
                PutPix(position.X + x, position.Y + y, color);
                this->mMouseCursorBufferAfter[x + y * 16] = GetPix(position.X + x, position.Y + y);
            }
        }
    }
    
    this->mMouseDrawn = true;
}
