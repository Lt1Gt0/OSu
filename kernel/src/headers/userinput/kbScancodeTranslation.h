#pragma once
#ifndef _KB_SCANCODERANSLATION_H_
#define _KB_SCANCODERANSLATION_H_

#include <types.h>

namespace QWERTYKeyboard
{
    constexpr u8 BackSpace   {0x0E}; 
    constexpr u8 Enter       {0x1C};
    constexpr u8 LeftShift   {0x2A};
    constexpr u8 RightShift  {0x36};
    constexpr u8 SpaceBar    {0x39};  
	constexpr u8 Special	 {0xE0};

	constexpr u8 CursorUp	 {0x48};
	constexpr u8 CursorRight {0x4D};
	constexpr u8 CursorLeft	 {0x4B};
	constexpr u8 CursorDown	 {0x50};

    extern const char ASCIITable[];
    char Translate(uint8 scancode, bool uppercase);
}

#endif // _KB_SCANCODERANSLATION_H_
