#include "IO.hpp"
#include "Typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

namespace IO{
	void remapPIC(){
		uint8 a1, a2;

		a1 = inb(PIC1_DATA);
		a2 = inb(PIC2_DATA);

		outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
		outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
		outb(PIC1_DATA, 0);
		outb(PIC2_DATA, 8);
		outb(PIC1_DATA, 4);
		outb(PIC2_DATA, 2);
		outb(PIC1_DATA, ICW4_8086);
		outb(PIC2_DATA, ICW4_8086);

		outb(PIC1_DATA, a1);
		outb(PIC2_DATA, a2);
	}
	
	void outb(uint16 port, uint8 val){
		asm volatile("out %1, %0" : : "a"(val), "Nd"(port));
	}

	uint8 inb(uint16 port){
		uint8 returnVal;
		asm volatile ("in %0, %1" 
			: "=a"(returnVal)
			: "Nd"(port));
		return returnVal;
	}
}