/*** Includes ***/
// Need to fix/rework code to not have to include all files
#include "Terminal.cpp"
#include "Color.h"
#include "IO/IO.cpp"
#include "Interrupts/IDT.h"
#include "Interrupts/IDT.cpp"
#include "Input/Keyboard.h"
#include "Input/Keyboard.cpp"
// #include "Memory/MemoryMap.hpp"
#include "Memory/MemoryMap.cpp"

extern "C" void _start(){
	// Initialize different things
	IDT::InitializeIDT();
	Terminal::InitializeTerminal();

	//Set the main keyboard handler of the IDT to the standard keyboard handler
	IDT::MainKeyboardHandler = Keyboard::standardKBHandler;
	Terminal::outputString(Terminal::intToString(MemoryMap::MemoryRegionCount));
	return;
}