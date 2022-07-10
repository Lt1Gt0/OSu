#pragma once
#ifndef _COLORS_H
#define _COLORS_H

#include <types.h>

namespace Colors 
{
    namespace TTY
    {
        enum : uint32 {
            OSU = 0xFFFF66AA,
            WYSI = 0x72727272,

            WHITE = 0xFFFFFFFF,
            RED = 0xFFFF0000,
            GREEN = 0xFF00FF00,
            BLUE = 0xFF0000FF,
            BLACK = 0x00000000,
        };
    }
}

#endif // _COLORS_H