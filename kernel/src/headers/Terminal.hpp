#pragma once
#include "Typedefs.h"
#include "Colors.h"

class Terminal{
public:
	Terminal();

	void InitializeTerminal();

	void SetCursorPosition(uint16 position);
	void OutputString(const char* str);

	uint8 CursorPositionCoords(uint8 x, uint8 y);

	template<typename T>
	const char* HexToString(T value);
};