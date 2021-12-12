#pragma once
#include "IO.cpp"
#define VGA_MEMORY (unsigned char*)0xb8000
#define VGA_WIDTH 160 
#define VGA_HEIGHT 50


unsigned short cursorPos;

//TODO Update cursor position based off of a scale of rows and columns
unsigned short scale_cursor_position(unsigned short x, unsigned short y){
	unsigned short row;
	unsigned short col;
	if(y > VGA_HEIGHT){
		// OVERFLOW OF ROWS
	}


	return (y * (VGA_WIDTH/2)) + (x/2);
}

void set_cursor_position(unsigned short x, unsigned short y){
	unsigned short position = scale_cursor_position(x, y);
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));

	cursorPos = position;
}
