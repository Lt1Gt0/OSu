#pragma once
#include "Typedefs.h"
#include "../Terminal.cpp"

namespace IDT{
	void InitializeIDT();
    extern "C" void isr1_handler(){
		Terminal::OutputString(Terminal::HexToString(inb(0x60)));
		IO::outb(0x20, 0x20);
		IO::outb(0xa0, 0x20);
	}
};