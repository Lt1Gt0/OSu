#include "Terminal.cpp"
#include "Color.h"

extern const char Test[];

extern "C" void _start(){
	Terminal terminal = Terminal();

	terminal.InitializeTerminal();

	terminal.OutputString(Test);
	return;
}