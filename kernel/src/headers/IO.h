#pragma once
#ifndef _IO_H_
#define _IO_H_

#include <types.h>

__attribute__((always_inline))
static inline void outb(uint16 port, byte value)
{
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

__attribute__((always_inline))
static inline byte inb(uint16 port)
{
    byte returnVal;
    asm volatile("inb %1, %0"
        : "=a"(returnVal)
        : "Nd"(port));
    return returnVal;
}

__attribute__((always_inline))
static inline void io_wait()
{
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

#endif // _IO_H_
