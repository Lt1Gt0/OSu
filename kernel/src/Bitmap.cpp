#include "Bitmap.h"

bool Bitmap::operator[](uint64_t index){
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex= index % 8;

    //Shifting the bit in the indexer to the right the amount in the bit index will return the value of the desired bit
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    if((Buffer[byteIndex] & bitIndexer) > 0){
        return true;
    }

    return false;
}

void Bitmap::Set(uint64_t index, bool value){
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex= index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;


    Buffer[byteIndex] &= ~bitIndexer;
    if(value){ // Not off
        Buffer[byteIndex] |= bitIndexer;
    }
}