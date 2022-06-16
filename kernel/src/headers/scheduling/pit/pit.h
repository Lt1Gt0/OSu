#pragma once
#ifndef _PIT_H
#define _PIT_H

#include <stdint.h>

namespace PIT
{
    #define CHANNEL_0           0x40
    #define CHANNEL_1           0x41
    #define CHANNEL_2           0x42
    #define MODE_COMMAND_REG    0x43

    extern double TimeSinceBoot;
    const uint64_t BaseFrequency = 1193182;

    void Sleepd(double seconds);
    void Sleep(uint64_t milliseconds);

    void SetDivisor(uint16_t divisor);
    uint64_t GetFrequency();
    void SetFrequecy(uint64_t frequency);
    void Tick();
    unsigned ReadCount();
}

#endif // _PIT_H