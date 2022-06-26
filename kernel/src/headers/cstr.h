#pragma once
#ifndef _CSTR_H
#define _CSTR_H

#include <stdint.h>
#include <stddef.h>

/*** Integer to string ***/
const char* to_string(uint64_t value);
const char* to_string(int64_t value);

/*** Hex to string ***/
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);

/*** Double/Float to string ***/
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_string(double value);

int memcmp(const void* aptr, const void* bptr, size_t size);

#endif // _CSTR_H