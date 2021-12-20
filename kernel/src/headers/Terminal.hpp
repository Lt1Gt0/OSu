#pragma once
#include "Typedefs.h"
#include "Color.h"

namespace Terminal{
	/**
 	 * Initialize termial window with default values
 	 */
	void InitializeTerminal();

	/**
 	 * Clear the terminal
 	 * 
 	 * @param color (optional) - foreground and background color 
 	 * 							 to clear the screen with (default to black on white)
 	 */
	void ClearTerminal(uint64 color = Color::BG_BLUE | Color::FG_WHITE);

	/**
 	 * Set the cursor position on the terminall window
 	 * 
 	 * @param position - location of the cursor on terminal window
 	 */
	void SetCursorPosition(uint16 position);

	/**
 	 * Output a string value to the terminal window
 	 * 
 	 * @param str - String value to be printed
 	 * @param color (optional) - foreground and background color 
 	 * 							 to clear the screen with (default to black on white)
 	 */
	void OutputString(const char* str, uint8 color = Color::FG_WHITE | Color::BG_BLUE);

	/**
	 * Display a single char onto the terminal window
	 * 
	 * @param chr - character value to output
	 * @param color (optional) - foreground and background color 
 	 * 							 to clear the screen with (default to black on white)
	 */
	void OutputChar(char chr, uint8 color = Color::FG_WHITE | Color::BG_BLUE);

	/**
	 * Return an (x,y) position as a mapped coordinate
	 * on the terminal window
	 * 
	 * @param x, y - Location horizontal and vertical on terminal window
	 * 
	 * @return - location in terminal the coresponds to the mapped coordinated
	 */
	uint8 CursorPositionCoords(uint8 x, uint8 y);

	/**
 	 * return a hex value input as a string output
 	 * 
 	 * @param T - hex value
 	 * @return char* that can be printed on terminal window
 	 */
	template<typename T>
	const char* HexToString(T value);
}