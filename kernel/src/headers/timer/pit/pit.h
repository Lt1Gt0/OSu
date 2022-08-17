#pragma once
#ifndef _PIT_H
#define _PIT_H

#include <types.h>

namespace PIT
{
    constexpr byte CHANNEL_0			{0x40}; 
    constexpr byte CHANNEL_1            {0x41}; 
    constexpr byte CHANNEL_2            {0x42}; 
    constexpr byte MODE_COMMAND_REG     {0x43}; 
	constexpr uint64 BASE_FREQUENCY		{1193182};
    extern double TimeSinceBoot;

    void Sleepd(double seconds);
    void Sleep(uint64 milliseconds);

    void SetDivisor(uint16 divisor);
    uint64 GetFrequency();
    void SetFrequecy(uint64 frequency);
    void Tick();
    unsigned ReadCount();
}

#endif // _PIT_H
