#include "IDT.hpp"

namespace IDT{
	IDT64 _idt[256];
	void InitializeIDT(){
		_idt[1].zero = 0;
		_idt[1].offset_low = (uint16)(((uint64)&isr1 & 0x000000000000ffff));
		_idt[1].offset_mid = (uint16)(((uint64)&isr1 & 0x00000000ffff0000) >> 16);
		_idt[1].offset_high = (uint32)(((uint64)&isr1 & 0xffffffff00000000) >> 32);
		_idt[1].ist = 0;
		_idt[1].selector = 0x08;
		_idt[1].types_attrs = 0x8e;
		
		IO::remapPIC();

		IO::outb(0x21, 0xfd);
		IO::outb(0xa1, 0xff);
		loadIDT();
	}
	
	void (*MainKeyboardHandler)(byte scanCode, byte chr);

	extern "C" void isr1_handler(){
		byte scanCode = IO::inb(0x60);
		byte chr = 0;
		if(MainKeyboardHandler != 0){
			MainKeyboardHandler(scanCode, chr);
		}
		if(scanCode < 0x3A){
			Terminal::outputChar(ScancodeTranslator::QWERTYLookupTable[scanCode]);
		}
		// IO for keyboard handler
		IO::outb(0x20, 0x20);
		IO::outb(0xa0, 0x20);
	}
}