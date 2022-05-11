#pragma once
#ifndef _SIMPLE_FONTS_H
#define _SIMPLE_FONTS_H

struct PSF1_HEADER {
	unsigned char magic[2]; // Identifier bytes
	unsigned char mode; // mode that the psf font is in
	unsigned char charsize; // how large the character are in bytes
};

struct PSF1_FONT {
	PSF1_HEADER* 	psf1_Header;
	void* 			glyphBuffer;
};

#endif // _SIMPLE_FONTS_H