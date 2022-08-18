#include <bitmap.h>

bool Bitmap::operator[](uint64 index)
{
    return Get(index);
}

bool Bitmap::Get(uint64 index)
{
    if (index > this->mSize * 8)
        return false; // Prevent buffer overflow

    uint64 byteIndex = index / 8;
    uint8 bitIndex= index % 8;

    //Shifting the bit in the indexer to the right the amount in the bit index will return the value of the desired bit
    uint8 bitIndexer = 0b10000000 >> bitIndex;
    if ((this->mBuffer[byteIndex] & bitIndexer) > 0)
        return true;

    return false;
}

bool Bitmap::Set(uint64 index, bool value)
{
    if (index > this->mSize * 8)
        return false; // Prevent buffer overflow
    
    uint64 byteIndex = index / 8;
    uint8 bitIndex= index % 8;
    uint8 bitIndexer = 0b10000000 >> bitIndex;

    this->mBuffer[byteIndex] &= ~bitIndexer;
    if (value) // Not off
        this->mBuffer[byteIndex] |= bitIndexer;

    return true;
}
