#pragma once 

#include <Terminal.hpp>
#include <Typedefs.h>
#include "IO/IO.hpp" 
#define VGA_MEMORY (uint8*)0xb8000 
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

uint8 cursorPos;

Terminal::Terminal(){
	SetCursorPosition(0);
}

void Terminal::SetCursorPosition(uint16 position){
	if(position >= 2000) // Check if the wanted position is out of bounds
		position = 1999; 

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8)(position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8)((position >> 8) & 0xFF));

	cursorPos = position;
}

void Terminal::PrintString(const char* str){
	uint8* charPtr = (uint8*)str;
	uint16 index = cursorPos;

	while(*charPtr != 0){

		switch(*charPtr){
			case 0x0a: //New Line
				index += VGA_WIDTH;
				break;
			case 0x0d: //Return Carriage
				index -= index % VGA_WIDTH; //TODO FIX
				break;
			default:
				*(VGA_MEMORY + index * 2) = *charPtr;
				index++;
		}
		charPtr++;
	}

	SetCursorPosition(index);
}

uint8 Terminal::CursorPositionCoords(uint8 x, uint8 y){
	// Might need ??
	// if(y >= VGA_HEIGHT)
	// 	y = VGA_HEIGHT - 1; //Set the Cursor y to the maximum it could be
	// if(x >= VGA_WIDTH)
	// 	x = VGA_WIDTH - 1; //Set the Cursor x to the maximum it could be

	return (y * VGA_WIDTH) + x;
}