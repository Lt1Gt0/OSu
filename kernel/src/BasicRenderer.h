#pragma once
#include "math.h"
#include "FrameBuffer.h"
#include "SimpleFonts.h"
#include <stdint.h>

class BasicRenderer{
    public:
    BasicRenderer(FrameBuffer* targetFrameBuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    FrameBuffer* TargetFrameBuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Color;
    unsigned int ClearColor;
    uint32_t MouseCursorBuffer[16 * 16];
    uint32_t MouseCursorBufferAfter[16 * 16];
    bool MouseDrawn;
    
    /**
     * @brief Print a string value to the render buffer
     * 
     * @param str char* string value
     */
    void Print(const char* str);

    /**
     * @brief Print a single char value to the render buffer
     * 
     * @param chr character to output
     * @param xOff offset of the x position for the char
     * @param yOff offset of the y position for the char
     */
    void PutChar(char chr, unsigned int xOff, unsigned int yOff); 
    void PutChar(char chr);

    void ClearChar();

    /**
     * @breif Clear terminal to a single color
     *
     * @param color
     */
    void Clear();
    
    /**
     * @brief Console new line
     */
    void Next();
    

    void ClearMouseCursor(uint8_t* mouseCursor, Point position);
    uint32_t GetPix(uint32_t x, uint32_t y);
    void PutPix(uint32_t x, uint32_t y, uint32_t color);
    void DrawOverlayMouseCursor(uint8_t* MouseCursor, Point position, uint32_t color);
};

extern BasicRenderer* GlobalRenderer;
