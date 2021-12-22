#include "../Input/Keyboard.h"
#include "Terminal.hpp"
#include "Typedefs.h"

namespace Keyboard{
	void standardKBHandler(byte scanCode, byte chr){
		if(chr != 0){
			switch(leftShiftPressed | rightShiftPressed){
				case true:
					Terminal::outputChar(chr - 32);
					break;
				case false:
					Terminal::outputChar(chr);
					break;
			}
		}
		else{
			switch(scanCode){
				/* Backspace */
				case 0x0E: //Pressed
					Terminal::setCursorPosition(Terminal::cursorPos-1);
					Terminal::outputChar(' ');
					break;
				case 0x8E: //released
					Terminal::setCursorPosition(Terminal::cursorPos-2);
					Terminal::outputChar(' ');
					Terminal::setCursorPosition(Terminal::cursorPos-2);
					break;

				/* Left Shift */
				case 0x2A: //Pressed
					leftShiftPressed = true;
					break;
				case 0xAA: //Released
					leftShiftPressed = false;
					break;

				/* Right Shift */
				case 0x36: //Pressed
					rightShiftPressed = true;
					break;
				case 0xB6: //Released
					rightShiftPressed = false;
					break;

				/* Enter */
				case 0x9C:
					Terminal::outputString("\n\r");
					break;
			}
		}
	}

	void kbHandler0xE0(byte scanCode){
		switch(scanCode){
			case 0x50: //Down arrow
				Terminal::setCursorPosition(Terminal::cursorPos + VGA_WIDTH);
				break;
			case 0x48: //up arrow
				Terminal::setCursorPosition(Terminal::cursorPos + VGA_WIDTH);
				break;
			default:
				break;
		}	
	}

	void specialKbHandler(byte scanCode, byte chr){
		switch(lastScanCode){
			case 0xE0:
				kbHandler0xE0(scanCode);
			default:
				standardKBHandler(scanCode, chr);
		}
		lastScanCode = scanCode;
	}
}