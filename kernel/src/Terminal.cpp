#pragma once 

/*** Includes ***/
#include <Terminal.hpp>
#include <Typedefs.h>
#include <Color.h>
#include "IO/IO.hpp" 

/*** Defines ***/

using namespace IO;

extern const char AboutOS[];
extern const char OSU_Logo[];

namespace Terminal{
	void InitializeTerminal(){
		clearTerminal();
		displaySplashScreen();
		setCursorPosition(cursorPos + VGA_WIDTH - (cursorPos % VGA_WIDTH));
	}
	
	void clearTerminal(uint64 color){
		uint64 val = 0;
		val += color << 8;
		val += color << 24;
		val += color << 40;
		val += color << 56;
		for(uint64* i = (uint64*)VGA_MEMORY; i < (uint64*)(VGA_MEMORY + 4000); i++){
			*i = val;
		}
	}

	void displaySplashScreen(){
		byte logoSize = 84;

		mForegroundColor = Color::FG_WHITE;
		mBackgroundColor = Color::BG_PINK;

		outputString(OSU_Logo, logoSize);
		mBackgroundColor = Color::BG_BLUE;

		outputString("\n\r");
		outputString(AboutOS);
	}

	void setCursorPosition(uint16 position){
		// Check if the wanted position is out of bounds
		if(position >= 2000) position = 1999; 
	
		outb(0x3D4, 0x0F);
		outb(0x3D5, (byte)(position & 0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (byte)((position >> 8) & 0xFF));
	
		cursorPos = position;
	}
	
	size_t len(const char* str){
		size_t i = 0;
		while(str[i]) i++;
		return i;
	}

	inline void outputString(const char* str){
		outputString(str, len(str));
	}

	void outputString(const char* str, size_t length){
		uint16 index = cursorPos;
	
		for(size_t i = 0; i < length; i++){
			switch(str[i]){
				case 0x00:
					index++;
					break;
				case 0x0a: //New Line
					index += VGA_WIDTH;
					index -= index % VGA_WIDTH;
					break;
				case 0x0d: //Return Carriage
					index -= index % VGA_WIDTH;
					break;
				default:
					*(VGA_MEMORY + index * 2) = str[i];
					*(VGA_MEMORY + index * 2 + 1) = mForegroundColor | mBackgroundColor;
					index++;
					break;
			}
		}
		setCursorPosition(index);
	}

	void outputChar(char chr, byte color){
		*(VGA_MEMORY + cursorPos * 2) = chr;
		*(VGA_MEMORY + cursorPos * 2 + 1) = color;
		setCursorPosition(cursorPos+1);
	}
	
	byte cursorPositionCoords(byte x, byte y){
		// Might need ??
		// if(y >= VGA_HEIGHT)
		// 	y = VGA_HEIGHT - 1; //Set the Cursor y to the maximum it could be
		// if(x >= VGA_WIDTH)
		// 	x = VGA_WIDTH - 1; //Set the Cursor x to the maximum it could be
	
		return (y * VGA_WIDTH) + x;
	}
	
	char hexToStringOutput[128];
	template<typename T>
	const char* HexToString(T value){
		T* valPtr = &value;
		byte* ptr;
		byte temp;
		byte size = (sizeof(T)) * 2 - 1;
	
		for(byte i = 0; i < size; i++){
			ptr = ((byte*)valPtr + i);
			temp = ((*ptr & 0xF0) >> 4);
			hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
			temp = (*ptr & 0x0F);
			hexToStringOutput[size - (i * 2)] = temp + (temp > 9 ? 55 : 48);
		}
		hexToStringOutput[size + 1] = 0;
		return hexToStringOutput;
	}
}