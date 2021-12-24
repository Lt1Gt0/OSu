#pragma once
#include "Typedefs.h"
#include "../Terminal.cpp"
#include "../IO/ScancodeTranslator.cpp"

namespace IDT{
	
	/**
	 * @brief Initialize the functions and paramaters
	 * of the IDT
	 */
	void InitializeIDT();

	/**
	 * @brief set the main keyboard handler pointer
	 */
    void (*MainKeyboardHandler)(byte scanCode, byte chr);

	/**
	 * @brief handle the first isr according to the
	 * qwerty keyboard layout
	 */
    extern "C" void isr1_handler(){
    	byte scanCode = inb(0x60);
   		byte chr = 0;

		if(MainKeyboardHandler != 0){
			MainKeyboardHandler(scanCode, chr);
		}
    	if(scanCode < 0x3A){
			Terminal::outputChar(SCTranslator::QWERTYLookupTable[scanCode]);
		}
		// IO for keyboard handler
		IO::outb(0x20, 0x20);
		IO::outb(0xa0, 0x20);
	}
};