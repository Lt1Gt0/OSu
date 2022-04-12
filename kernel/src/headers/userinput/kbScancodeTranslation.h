#pragma once
#include <stdint.h>

namespace QWERTYKeyboard
{
    #define BackSpace   0x0E
    #define Enter       0x1C
    #define LeftShift   0x2A
    #define RightShift  0x36
    #define SpaceBar    0x39  

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}
