#include "cstr.h"

char uintTo_StringOutput[128]; // Buffer to hold characters in
const char *to_string(uint64_t value)
{
    uint8_t size; // Size of the string that will be produced
    uint64_t sizeTest = value;
    
    //Get the size of the string
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0; // Buffer index
    while (value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0'; // Working backwards
        index++;
    }

    uint8_t remainder = value % 10; // Finalize value
    uintTo_StringOutput[size - index] = remainder + '0';
    uintTo_StringOutput[size + 1] = 0; // Null termination
    return uintTo_StringOutput;
}

char intTo_StringOutput[128]; // Buffer to hold characters in
const char *to_string(int64_t value)
{
    uint8_t isNegative = 0;
    if (value < 0) {
        isNegative = 1;
        value *= -1;
        intTo_StringOutput[0] = '-';
    }

    uint8_t size; // Size of the string that will be produced
    uint64_t sizeTest = value;
    
    //Get the size of the string
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0; // Buffer index
    while (value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10;
        intTo_StringOutput[isNegative + size - index] = remainder + '0'; // Working backwards
        index++;
    }

    uint8_t remainder = value % 10; // Finalize value
    intTo_StringOutput[isNegative + size - index] = remainder + '0';
    intTo_StringOutput[isNegative + size + 1] = 0; // Null termination
    return intTo_StringOutput;
}

char hexTo_StringOutput64[128];
const char *to_hstring(uint64_t value)
{
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput64[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput64[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hexTo_StringOutput64[size + 1] = 0; //Null termination
    return hexTo_StringOutput64;
}

char hexTo_StringOutput32[128];
const char *to_hstring(uint32_t value)
{
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hexTo_StringOutput32[size + 1] = 0; //Null termination
    return hexTo_StringOutput32;
}

char hexTo_StringOutput16[128];
const char *to_hstring(uint16_t value)
{
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hexTo_StringOutput16[size + 1] = 0; //Null termination
    return hexTo_StringOutput16;
}

char hexTo_StringOutput8[128];
const char *to_hstring(uint8_t value)
{
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hexTo_StringOutput8[size + 1] = 0; //Null termination
    return hexTo_StringOutput8;
}

char doubleTo_StringOutput[128];
const char *to_string(double value, uint8_t decimalPlaces)
{
    if (decimalPlaces > 20)
        decimalPlaces = 20;

    char* intPtr = (char*)to_string((int64_t)value);
    char* doublePtr = doubleTo_StringOutput;

    if (value < 0)
        value *= -1;

    while (*intPtr != 0) {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newVal = value - (int)value; // Take out the decimal portion

    for (uint8_t i = 0; i < decimalPlaces; i++) {
        newVal *= 10;
        *doublePtr = (int)newVal + '0';
        newVal -= (int)newVal;
        doublePtr++;
    }

    //Finialize string
    *doublePtr = 0;
    return doubleTo_StringOutput;
}

const char *to_string(double value)
{
    return to_string(value, 2);
}