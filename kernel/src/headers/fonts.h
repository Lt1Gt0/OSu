#pragma once
#ifndef _SIMPLE_FONTS_H_
#define _SIMPLE_FONTS_H_

#include <types.h>

struct PSF1_HEADER {
	byte magic[2]; // Identifier bytes
	byte mode; // mode that the psf font is in
	byte charsize; // how large the character are in bytes
};

struct PSF1_FONT {
	PSF1_HEADER* 	psf1Header;
	void* 			glyphBuffer;
};

#endif // _SIMPLE_FONTS_H_
