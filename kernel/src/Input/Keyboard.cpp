#include "../Input/Keyboard.h"

bool ls_pressed = false;
bool rs_pressed = false;
byte lastScanCode;

namespace Keyboard{
	void standardKBHandler(byte scanCode, byte chr) {
		if(chr != 0){
			//Check if either left shift or right shift is pressed
			switch(ls_pressed | rs_pressed){					
				//If so, capitialize the character by subtracting 32 from the chr byte
				case true:
					Terminal::outputChar(chr - 32);
					break;
				//If not, print the same character as lowercase
				case false:
					Terminal::outputChar(chr);
					break;
			}
		} else {
			byte currentRow = Terminal::cursorPos / VGA_WIDTH;

			// Check scancode cases
			switch(scanCode){
				case BACKSPACE_PRESSED:
					Terminal::setCursorPosition(Terminal::cursorPos - 1);
					break;
				case BACKSPACE_RELEASED:
					Terminal::setCursorPosition(Terminal::cursorPos - 1);
					Terminal::outputChar(' ');
					Terminal::setCursorPosition(Terminal::cursorPos - 1);
					break;
				case L_SHIFT_PRESSED:
					ls_pressed = true;
					break;
				case L_SHIFT_RELEASED:
					ls_pressed = false;
				case R_SHIFT_PRESSED:
					rs_pressed = true;
					break;
				case R_SHIFT_RELEASED:
					rs_pressed = false;
				case PAGE_UP_PRESSED:
					Terminal::setCursorPosition(Terminal::cursorPos - (currentRow * VGA_WIDTH));
					break;
				case PAGE_DOWN_PRESSED:
					Terminal::setCursorPosition(Terminal::cursorPos + ((VGA_HEIGHT-currentRow-1) * VGA_WIDTH));
					break;
				case HOME_PRESSED:
					Terminal::setCursorPosition(Terminal::cursorPos - (Terminal::cursorPos % VGA_WIDTH));
					break;
				case END_PRESSED:
					Terminal::setCursorPosition(Terminal::cursorPos - 1 + VGA_WIDTH - (Terminal::cursorPos % VGA_WIDTH)); // Could be reworked
					break;

				case ENTER_PRESSED:
					Terminal::outputString("\r\n");
					break;
			}
		}
	}

	void kbHandler0xE0(byte scanCode){
		switch(scanCode){

			/*** TODO ***/
			case ARROW_DOWN:
				Terminal::setCursorPosition(Terminal::cursorPos + VGA_WIDTH);
				break;
			case ARROW_UP:
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