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
	
	
	uint64* TestAddress = (uint64*)aligned_alloc(0x4000, 0x08);
	Terminal::outputString(Terminal::hexToString((uint64)TestAddress));
	Terminal::outputString("\n\r");
	free(TestAddress);

	uint64* TestAddress2 = (uint64*)malloc(0x4000);
	Terminal::outputString(Terminal::hexToString((uint64)TestAddress2));
	Terminal::outputString("\n\r");

	return;
}