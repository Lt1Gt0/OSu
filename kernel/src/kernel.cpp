#include "Terminal.cpp"

extern "C" void _start(){
	Terminal terminal = Terminal();

	terminal.InitializeTerminal();
	terminal.PrintString("Hello World");
	return;
}