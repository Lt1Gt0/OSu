#pragma once
#include "Typedefs.h"
#include "../Terminal.cpp"
#include "../IO/ScancodeTranslator.cpp"

namespace IDT{
	void InitializeIDT();
    extern "C" void isr1_handler(){
    	uint8 scanCode = inb(0x60);
    	if(scanCode < 0x3A){
			Terminal::OutputChar(SCTranslator::QWERTYLookupTable[scanCode]);
		}
		IO::outb(0x20, 0x20);
		IO::outb(0xa0, 0x20);
	}
};