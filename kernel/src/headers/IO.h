#pragma once
#include "Typedefs.h"

namespace IO{

	#define PIC1_COMMAND 0x20
	#define PIC1_DATA 0x21
	#define PIC2_COMMAND 0xA0
	#define PIC2_DATA 0xA1
	
	#define ICW1_INIT 0x10
	#define ICW1_ICW4 0x01
	#define ICW4_8086 0x01

	/**
	 * @brief Remap the PIC
	 */
	void remapPIC();

	/**
	 * @brief send byte out into a given port
	 * 
	 * @param port port to send out bytes to
	 * @param val Value to send to the port 
	 */
	void outb(uint16 port, uint8 val);

	/**
	 * @brief Get incoming bytes from a port
	 * 
	 * @param port port to get incoming bytes from
	 * @return uint8 
	 */
	uint8 inb(uint16 port);
}

