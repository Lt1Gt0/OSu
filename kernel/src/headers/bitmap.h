#pragma once
#ifndef _BITMAP_H
#define _BITMAP_H

#include <types.h>

class Bitmap
{
    public:
        size_t mSize; //Size of the buffer the bitmap will use
        uint8* mBuffer; //where the bits will be stored
        bool operator[](uint64 index); //Set up a '[]' operator for indexing

        /**
         * @brief Set a value at a given index denoted by the '[]' operator
         * 
         * @param index Index of the bitmap desired
         * @param value The value to set at the given index
         */
        bool Set(uint64 index, bool value);
        bool Get(uint64 index);
};

#endif // _BITMAP_H
