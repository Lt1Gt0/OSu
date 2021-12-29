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
	
	uint64* TAddress = (uint64*)malloc(0x08);
	*TAddress = 12345678;
	Terminal::outputString(Terminal::intToString(*TAddress));
	Terminal::outputString("\n\r");

	uint64* TAddress2 = (uint64*)realloc(TAddress,0x08);
	Terminal::outputString(Terminal::intToString(*TAddress2));
	Terminal::outputString("\n\r");

	uint64* TAddress3 = (uint64*)calloc(0x08);
	Terminal::outputString(Terminal::intToString(*TAddress3));
	Terminal::outputString("\n\r");
	
	return;
}