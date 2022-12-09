#pragma once
#ifndef _BITMAP_H
#define _BITMAP_H

#include <stddef.h>
#include <stdint.h>

class Bitmap
{
    public:
        size_t Size; //Size of the buffer the bitmap will use
        uint8_t* Buffer; //where the bits will be stored
        bool operator[](uint64_t index); //Set up a '[]' operator for indexing

        /**
         * @brief Set a value at a given index denoted by the '[]' operator
         * 
         * @param index Index of the bitmap desired
         * @param value The value to set at the given index
         */
        bool Set(uint64_t index, bool value);
        bool Get(uint64_t index);
};

#endif // _BITMAP_H
