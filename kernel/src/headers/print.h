#pragma once
#ifndef _KERNEL_PRINT_H
#define _KERNEL_PRINT_H

#include <BasicRenderer.h>
#include <string.h>

__attribute__((always_inline)) inline void kprintf(const char* fstr, nint p0 = 0, nint p1 = 0, nint p2 = 0, nint p3 = 0, nint p4 = 0, nint p5 = 0, nint p6 = 0, nint p7 = 0, nint p8 = 0, nint p9 = 0)
{
	GlobalRenderer.Print(format(fstr, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9));
}

#endif // _KERNEL_PRINT_H