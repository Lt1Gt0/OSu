#pragma once
#include "Typedefs.h"

namespace ScancodeTranslator{
	const uint8 QWERTYLookupTable[] = {
		0x0, 0x0, '1', '2',
		'3', '4', '5', '6',
		'7', '8', '9', '0',
		'-', '=', 0x0, 0x0,
		'q', 'w', 'e', 'r',
		't', 'y', 'u', 'i',
		'o', 'p', '[', ']',
		0x0, 0x0, 'a', 's',
		'd', 'f', 'g', 'h',
		'j', 'k', 'l', ';',
		'\'', '`', 0x0, '\\',
		'z', 'x', 'c', 'v',
		'b', 'n', 'm', ',',
		'.', '/', 0x0,  '*',
		0x0, ' '
	};

	/**
	 * @brief Scan code loop up table to grab from 
	 */
	extern const uint8 ScanCodeLookupTable[];
}