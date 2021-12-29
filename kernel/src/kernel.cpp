#include "Interrupts/IDT.hpp"
#include "Input/Keyboard.h"
#include "MemoryMap.h"
#include "Heap.h"

extern "C" void _start(){
	// Initialize different things
	IDT::InitializeIDT();
	Terminal::InitializeTerminal();
	Terminal::clearTerminal();
	Terminal::setCursorPosition(0);
	//Set the main keyboard handler of the IDT to the standard keyboard handler
	IDT::MainKeyboardHandler = Keyboard::standardKBHandler;
	
	MemoryMap::MemoryMapEntry** usableMemoryMaps = MemoryMap::getUsableMemoryRegions();

	InitializeHeap(0x100000, 0x100000);
	void* TestMemoryAddress1 = malloc(0x10);
	void* TestMemoryAddress2 = malloc(0x10);
	void* TestMemoryAddress3 = malloc(0x10);
	Terminal::outputString(Terminal::hexToString((uint64)TestMemoryAddress1));
	Terminal::outputString("\n\r");
	Terminal::outputString(Terminal::hexToString((uint64)TestMemoryAddress2));
	Terminal::outputString("\n\r");
	Terminal::outputString(Terminal::hexToString((uint64)TestMemoryAddress3));
	return;
}