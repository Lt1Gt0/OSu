#pragma once
#include "IO.cpp"
#include "Typedefs.cpp"
#define VGA_MEMORY (uint_8*)0xb8000
#define VGA_WIDTH 80 
#define VGA_HEIGHT 25


uint_8 cursorPos;

uint_8 scale_cursor_position(uint_8 x, uint_8 y){
	unsigned short row;
	unsigned short col;

	// Might need ??
	// if(y >= VGA_HEIGHT)
	// 	y = VGA_HEIGHT - 1; //Set the Cursor y to the maximum it could be
	// if(x >= VGA_WIDTH)
	// 	x = VGA_WIDTH - 1; //Set the Cursor x to the maximum it could be

	return (y * VGA_WIDTH) + x;
}

void set_cursor_position(uint_16 position){
	if(position >= 2000) // Check if the wanted position is out of bounds
		position = 1999; 

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint_8)(position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint_8)((position >> 8) & 0xFF));

	cursorPos = position;
}