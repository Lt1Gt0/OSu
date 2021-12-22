/*** Includes ***/
#include "Terminal.cpp"
#include "Color.h"
#include "IO/IO.cpp"
#include "Interrupts/IDT.h"
#include "Interrupts/IDT.cpp"
#include "Input/Keyboard.h"
#include "Input/Keyboard.cpp"

extern "C" void _start(){
	// Initialize different things
	IDT::InitializeIDT();
	Terminal::InitializeTerminal();

	IDT::MainKeyboardHandler = Keyboard::standardKBHandler;

	return;
}