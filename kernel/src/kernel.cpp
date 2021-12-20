#include "Terminal.cpp"
#include "Color.h"
#include "IDT.h"
#include "IO/IO.cpp"
#include "IDT.cpp"

extern "C" void _start(){
	// Initialize different things
	IDT::InitializeIDT();
	Terminal::InitializeTerminal();
	
	return;
}