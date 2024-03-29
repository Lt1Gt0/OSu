#pragma once
#ifndef _TYPES_H_
#define _TYPES_H_

#define PACKED	__attribute__((packed))
#define UNUSED	__attribute__((unused)) 

typedef signed char sbyte;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned long size_t;
typedef size_t nint;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

constexpr uint16 PAGE_SIZE {0x1000};

#endif // _TYPES_H_
