#pragma once
#include <stdint.h>

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