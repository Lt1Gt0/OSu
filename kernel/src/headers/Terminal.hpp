#pragma once
#include "Typedefs.h"
#include "Color.h"

namespace Terminal{
	#define VGA_MEMORY (byte*)0xb8000 
	#define VGA_WIDTH 80
	#define VGA_HEIGHT 25

	byte mBackgroundColor = Color::BG_BLUE;
	byte mForegroundColor = Color::FG_WHITE;
	uint64 cursorPos;

	/**
 	 * @brief Initialize termial window with default values
 	 */
	void InitializeTerminal();

	/**
 	 * @brief Clear the terminal
 	 * 
 	 * @param color (optional) foreground and background color 
 	 * to clear the screen with (default to black on white)
 	 */
	void clearTerminal(uint64 color = mBackgroundColor | mForegroundColor);

	/**
	 * @brief Display the splash screen on the terminal window
	 */
	void displaySplashScreen();

	/**
 	 * @brief Set the cursor position on the terminall window
 	 * 
 	 * @param position location of the cursor on terminal window
 	 */
	void setCursorPosition(uint16 position);

	/**
	 * @brief Return the length of a given string
	 */
	size_t len(const char* str);
	
	inline void outputString(const char* str);

	/**
 	 * @brief Output a string value to the terminal window
 	 * 
 	 * @param str String value to be printed
 	 * @param length length of the string
 	 */
	void outputString(const char* str, size_t length);

	/**
	 * Display a single char onto the terminal window
	 * 
	 * @param chr character value to output
	 * @param color (optional) foreground and background color 
 	 * to clear the screen with (default to black on white)
	 */
	void outputChar(char chr, byte color = mBackgroundColor | mForegroundColor);

	/**
	 * @brief Return an (x,y) position as a mapped coordinate
	 * on the terminal window
	 * 
	 * @param x Location horizontal in terminal window
	 * @param y Location vertical on terminal window
	 * 
	 * @return location in terminal the coresponds to the mapped coordinated
	 */
	byte cursorPositionCoords(byte x, byte y);

	/**
 	 * @brief return a hex value input as a string output
 	 * 
 	 * @param T hex value
 	 * @return const char*
 	 */
	template<typename T>
	const char* hexToString(T value);

	/**
	 * @brief Return an integer value input as a string output
	 * 
	 * @tparam T 
	 * @param Value integer value
	 * @return const char* 
	 */
	template<typename T>
	const char* intToString(T Value);

	/**
	 * @brief Return a floating point value to a string output
	 * based off of the amount of decimal places you would like
	 * to round to
	 * 
	 * @param value Floating point value input to convert to string 
	 * @param decimalPlaces The amount of decimal places of percision
	 * to be rounded to
	 * @return const char* 
	 */
	const char* floatToString(float value, byte decimalPlaces);
}