#pragma once
#ifndef _STRING_H_
#define _STRING_H_

#ifndef UNUSED
    #define UNUSED __attribute__((unused))
#endif // UNUSED

#include <types.h>

/*** Integer to string ***/
const char* tostr(uint64 value);
const char* tostr(int64 value);

/*** Hex to string ***/
const char* tohstr(uint64 value);
const char* tohstr(uint32 value);
const char* tohstr(uint16 value);
const char* tohstr(uint8 value);

/*** Double/Float to string ***/
const char* tostr(double value, uint8 decimalPlaces);
const char* tostr(double value);

int memcmp(const void* aptr, const void* bptr, size_t size);
void* memset(void* buf, int val, size_t size);
void* memcpy(void* dstptr, const void* srcptr, size_t size);
size_t strlen(const char* str);

/**
 * (Zman2024 My beloved)
 * 
 * @return const char* 
 */
const char* format(const char* fstring, nint p0 = 0, nint p1 = 0, nint p2 = 0, nint p3 = 0, nint p4 = 0, nint p5 = 0, 
					nint p6 = 0, nint p7 = 0, nint p8 = 0, nint p9 = 0);

#endif // _STRING_H_
