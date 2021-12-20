#pragma once
#include "Typedefs.h"

namespace IO{
	void remapPIC();
	void outb(uint16 port, uint8 val);
	uint8 inb(uint16 port);
}

