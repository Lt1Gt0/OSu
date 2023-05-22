#pragma once
#ifndef _PANIC_H_
#define _PANIC_H_

#include <interrupts/interrupts.h>

void Panic(InterruptFrame* interruptFrame, const char* panicMessage);

#endif // _PANIC_H_
