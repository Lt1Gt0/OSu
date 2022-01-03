#pragma once
#include "math.h"
#include "FrameBuffer.h"
#include "SimpleFonts.h"

class BasicRenderer{
    public:
    BasicRenderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    FrameBuffer* TargetFrameBuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Color;
    void Print(const char* str);
    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
};