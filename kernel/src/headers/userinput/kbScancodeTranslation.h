#pragma once
#include <stdint.h>

namespace QWERTYKeyboard
{
    #define BackSpace   0x0E
    #define Enter       0x1C
    #define LeftShift   0x2A
    #define RightShift  0x36
    #define SpaceBar    0x39  
	#define Special		0xE0

	#define CursorUp	0x48
	#define CursorRight	0x4D
	#define CursorLeft	0x4B
	#define CursorDown	0x50

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}
