#include "Terminal.cpp"

extern "C" void _start(){
	Terminal terminal = Terminal();

	terminal.InitializeTerminal();
	terminal.OutputString(terminal.HexToString(0x123478));
	return;
}