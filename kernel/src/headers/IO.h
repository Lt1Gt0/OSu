#pragma once
#ifndef _IO_H
#define _IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

#endif // _IO_H