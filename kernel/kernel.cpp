#include "PrintText.cpp"

extern "C" void _start(){
	set_cursor_position(scale_cursor_position(0, 0)); //Set the cursor to the top left by default
	return;
}