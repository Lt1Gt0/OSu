#include "Interrupts/IDT.hpp"
#include "Input/Keyboard.h"
#include "MemoryMap.h"

extern "C" void _start(){
	// Initialize different things
	IDT::InitializeIDT();
	Terminal::InitializeTerminal();
	Terminal::clearTerminal();
	Terminal::setCursorPosition(0);
	//Set the main keyboard handler of the IDT to the standard keyboard handler
	IDT::MainKeyboardHandler = Keyboard::standardKBHandler;
	
	MemoryMap::MemoryMapEntry** usableMemoryMaps = MemoryMap::getUsableMemoryRegions();

	for(byte i = 0; i < MemoryMap::usableMemoryRegionCount; i++){
		MemoryMap::MemoryMapEntry* MemoryMapLocation = usableMemoryMaps[i]; //Memory address was as defined in DetectMemory.asm
		printMemoryMap(MemoryMapLocation, Terminal::cursorPos);
		Terminal::outputString("\n\n\r");
	}
	return;
}