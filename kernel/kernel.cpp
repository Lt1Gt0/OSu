#include "PrintText.cpp"

extern "C" void _start(){
	SetCursorPosition(CursorPositionCoords(0,0)); //Set the cursor to the top left by default
	PrintString("Dice Roll Game\nBruh\nMy Cock burns alot");
	return;
}