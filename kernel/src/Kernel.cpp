#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"

extern "C" void _start(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font){
    BasicRenderer newRenderer = BasicRenderer(frameBuffer, psf1_font);
    newRenderer.Print(to_string((uint64_t)1234976));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_string((int64_t)-1234976));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_string((double)-13.14));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint64_t)0xF0));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint32_t)0xF0));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint16_t)0xF0));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint8_t)0xF0));
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};

    return;
}