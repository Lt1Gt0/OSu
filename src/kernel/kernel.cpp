#include "PrintText.cpp"

extern "C" void _start(){
	SetCursorPosition(CursorPositionCoords(0,0)); //Set the cursor to the top left by default
	PrintString("Hello World!");
	return;
}