#include <devices/audio/sb16.h>
#include <IO.h>
#include <BasicRenderer.h>
#include <cstr.h>

namespace SB16
{
    void ResetDSP()
    {
        outb(DSP_RESET_PORT, 1);
        // Sleep for 3 micro seconds
        // I currently dont have a uSleep function for the PIT
        outb(DSP_RESET_PORT, 0);
        uint8_t response = inb(DSP_READ_PORT);
        GlobalRenderer.Print("Response from DSP: ");
        GlobalRenderer.PrintLine(to_hstring(response));
    }

    void SetTransferMode(uint8_t mode)
    {

    }

    void SetSoundType(uint8_t type)
    {

    }

    void PlaySound()
    {
        ResetDSP();

        // Load Sound Data to memory

        // Make this as input parameter later (just for testing right now)
        uint8_t volume = 50;
        outb(DSP_MIXER_MASTER_VOLUME, volume);

        // Turn on speaker
        outb(DSP_WRITE_PORT, DSP_SPEAKER_ON);

        // Program ISA DMA to transfer

        // Set time constant

        // Set output sample rate

        // Write transfer mode to DSP

        // Write type of sound data

        // Write data lenght to DSP 

    }
}