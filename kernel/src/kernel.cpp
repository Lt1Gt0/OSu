#include "Terminal.cpp"

extern "C" void _start(){
	Terminal terminal = Terminal();
	terminal.PrintString("Among Us");
	return;
}