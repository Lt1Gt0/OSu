#include <timer/pit/pit.h>
#include <IO.h>

namespace PIT
{
    double TimeSinceBoot = 0;
    uint16 Divisor = 65535;

    void Sleepd(double seconds)
    {
        double startTime = TimeSinceBoot;
        while (TimeSinceBoot < startTime + seconds) {
            asm("hlt");
        }
    }

    void Sleep(uint64 milliseconds)
    {
        Sleepd((double)milliseconds / 1000);
    }

    void SetDivisor(uint16 divisor)
    {
        if (divisor < 100) 
            divisor = 100;
        
        Divisor = divisor;

        // Set the divisor for channel 0
        outb(CHANNEL_0, (uint8)(divisor & 0x00ff));           // Set low byte
        io_wait();
        outb(CHANNEL_0, (uint8)((divisor & 0xff00) >> 8));    // Set high byte
    }

    uint64 GetFrequency()
    {
        return BaseFrequency / Divisor;
    }

    void SetFrequency(uint64 frequency)
    {
        SetDivisor(BaseFrequency / frequency);
    }

    void Tick()
    {
        TimeSinceBoot += 1 / (double)GetFrequency();
    }

    unsigned ReadCount()
    {
        unsigned count = 0;

        // Disable interrupts
        asm("cli");

        // al = channel in bits 6 and 7, remaning bits clear
        outb(0x43, (uint8)0b0000000);

        count = inb(0x40);          // Low byte
        count |= inb(0x40) << 8;    // High byte

        return count;
    }
}