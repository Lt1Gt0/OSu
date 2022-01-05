#pragma once
#include <stddef.h>
#include <stdint.h>

class Bitmap{
    public:
    size_t Size; //Size of the buffer the bitmap will use
    uint8_t* Buffer; //where the bits will be stored
    bool operator[](uint64_t index);
    void Set(uint64_t index, bool value);
};