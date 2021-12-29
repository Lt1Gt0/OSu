#include "Terminal.hpp"

using namespace IO;

namespace Terminal{
	byte mBackgroundColor = Color::BG_BLUE;
	byte mForegroundColor = Color::FG_WHITE;
	uint16 cursorPos;

	void InitializeTerminal(){
		clearTerminal();
		displaySplashScreen();
		setCursorPosition(cursorPos + VGA_WIDTH - (cursorPos % VGA_WIDTH));
	}
	
	void clearTerminal(uint64 color){
		uint64 val = 0;

		//Bit shift left according to VGA color and characters
		val += color << 8;
		val += color << 24;
		val += color << 40;
		val += color << 56;

		//Clear each value in VGA Memory
		for(uint64* i = (uint64*)VGA_MEMORY; i < (uint64 *)(VGA_MEMORY + 4000); i++){
			*i = val;
		}
	}

	void displaySplashScreen(){
		//length of the osu size (static, might make it somehow dynamic)
		byte logoSize = 84;

		//Color of the slash screen
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

		//Tell IO for the terminal to move cursor with color value to the position
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
				case 0x00: //Null character
					index++;
					break;
				case '\n': //New Line
					index += VGA_WIDTH;
					index -= index % VGA_WIDTH;
					break;
				case '\r': //Return Carriage
					index -= index % VGA_WIDTH;
					break;
				default: //No special character
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
	
	// Hex buffer
	char hexToStringOutput[128];
	template<typename T>
	const char* hexToString(T value){
		T* valPtr = &value;
		byte* ptr;
		byte temp;
		byte size = (sizeof(T)) * 2 - 1;

		for(byte i = 0; i < size; i++){
			ptr = ((byte*)valPtr + i);
			temp = ((*ptr & 0xF0) >> 4);
			hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48); //Ascii range for hex values
			temp = (*ptr & 0x0F);
			hexToStringOutput[size - (i * 2)] = temp + (temp > 9 ? 55 : 48); //Ascii range for hex values
		}
		hexToStringOutput[size + 1] = 0;
		return hexToStringOutput;
	}

	const char* hexToString(uint8 value) { return hexToString<uint8>(value); }
	const char* hexToString(uint16 value) { return hexToString<uint16>(value); }
	const char* hexToString(uint32 value) { return hexToString<uint32>(value); }
	const char* hexToString(uint64 value) { return hexToString<uint64>(value); }
	const char* hexToString(char value) { return hexToString<char>(value); }
	const char* hexToString(short value) { return hexToString<short>(value); }
	const char* hexToString(int value) { return hexToString<int>(value); }
	const char* hexToString(long long value) { return hexToString<long long>(value); }

	// Integer buffer
	char intToStringOutput[128];
	template<typename T>
	const char* intToString(T value){
		//If the given integer is a negative value
		byte isNegative = 0;
		if (value < 0){
			isNegative = 1;
			value *= -1;
			intToStringOutput[0] = '-';
		}
		byte size = 0;
		uint64 sizeTester = (uint64)value;

		// Get the size of the integer's string equivalent
		while(sizeTester / 10 > 0){
			sizeTester /= 10;
			size++;
		}

		byte index = 0;
		uint64 newVal = (uint64)value;

		// Add the remainder value of the integer to the integer buffer until the value of the new integer is 0
		while(newVal / 10 > 0){
			byte remainder = newVal % 10;
			newVal /= 10;
			intToStringOutput[isNegative + size - index] = remainder + 48;
			index++;
		}
		// Last value of the integer
		byte remainder = newVal % 10;
		intToStringOutput[isNegative + size - index] = remainder + 48; // Resized for negative number
		intToStringOutput[isNegative + size + 1] = 0;				   // Resized for negative number
		return intToStringOutput;
	}
	
	const char* intToString(uint8 value) { return intToString<uint8>(value); }
	const char* intToString(uint16 value) { return intToString<uint16>(value); }
	const char* intToString(uint32 value) { return intToString<uint32>(value); }
	const char* intToString(uint64 value) { return intToString<uint64>(value); }
	const char* intToString(char value) { return intToString<char>(value); }
	const char* intToString(short value) { return intToString<short>(value); }
	const char* intToString(int value) { return intToString<int>(value); }
	const char* intToString(long long value) { return intToString<long long>(value); }

	// Floating point buffer
	char floatToStringOutput[128];
	const char* floatToString(float value, byte decimalPlaces){
		char* intPtr = (char*)intToString((int)value);
		char* floatPtr = floatToStringOutput;

		// Same thing as the ineger negaive check, except it can be reduced cause the intToDecimal
		// function is being used partially in this function
		if (value < 0) value *= -1;
		
		// get the integer value of everything to the left of the decimal place
		while(*intPtr != 0){
			*floatPtr = *intPtr;
			intPtr++;
			floatPtr++;
		}
		*floatPtr = '.';
		floatPtr++;

		float newVal = value - (int)value;
		//Get every integer value of everything to the right of the decimal place by multiplying the value by
		//10 and doing effectivly the same thing as before with everything to the left of the decimal
		for(byte i = 0; i < decimalPlaces; i++){
			newVal *= 10;
			*floatPtr = (int)newVal + 48;
			newVal -=(int)newVal;
			floatPtr++;
		}
		*floatPtr = 0;

		return floatToStringOutput;
	}

	const char* floatToString(uint8 value) { return floatToString(value); }
	const char* floatToString(uint16 value) { return floatToString(value); }
	const char* floatToString(uint32 value) { return floatToString(value); }
	const char* floatToString(uint64 value) { return floatToString(value); }
	const char* floatToString(char value) { return floatToString(value); }
	const char* floatToString(short value) { return floatToString(value); }
	const char* floatToString(int value) { return floatToString(value); }
	const char* floatToString(long long value) { return floatToString(value); }
}