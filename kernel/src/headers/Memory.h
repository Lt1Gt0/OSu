#pragma once
#include "Typedefs.h"

/**
 * @brief 
 * 
 * @param start 
 * @param value 
 * @param num 
 */
void memset(void* start, uint64 value, uint64 num);

/**
 * @brief 
 * 
 * @param destination 
 * @param source 
 * @param num 
 */
void memcpy(void* destination, void* source, uint64 num);