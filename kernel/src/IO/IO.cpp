#include "IO.hpp"
#include "Typedefs.h"

namespace IO{
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