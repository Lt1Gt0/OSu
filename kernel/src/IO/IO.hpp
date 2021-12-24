#pragma once
#include "Typedefs.h"

namespace IO{
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

