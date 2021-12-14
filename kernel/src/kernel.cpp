#include "Terminal.cpp"
#include "Color.h"

extern "C" void _start(){
	Terminal terminal = Terminal();

	terminal.InitializeTerminal();


	return;
}