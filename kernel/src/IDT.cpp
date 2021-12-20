#include <IDT.h>
#include <Typedefs.h>
#include <Terminal.hpp>
#include "IO/IO.hpp"

struct IDT64{
	uint16 offset_low;
	uint16 selector;
	uint8 ist;
	uint8 types_attrs;
	uint16 offset_mid;
	uint32 offset_high;
	uint32 zero;
};

extern IDT64 _idt[256];
extern uint64 isr1;
extern "C" void LoadIDT();

namespace IDT{
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
		LoadIDT();
	}
	
}