#pragma once
#include "Typedefs.h"
#include "Terminal.hpp"
#include "ScancodeTranslator.h"
#include "IO.h"

namespace IDT{

	struct IDT64{
		uint16 offset_low;
		uint16 selector;
		byte ist;
		byte types_attrs;
		uint16 offset_mid;
		uint32 offset_high;
		uint32 zero;
	};

	extern "C" IDT64 _idt[256];
	extern "C" uint64 isr1;
	extern "C" void loadIDT();
	
	/**
	 * @brief Initialize the functions and paramaters
	 * of the IDT
	 */
	void InitializeIDT();
	/**
	 * @brief set the main keyboard handler pointer
	 */
	extern "C" void (*MainKeyboardHandler)(byte scanCode, byte chr);
	/**
	 * @brief handle the first isr according to the
	 * qwerty keyboard layout
	 */
}