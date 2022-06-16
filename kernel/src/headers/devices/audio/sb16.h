#pragma once
#ifndef _SOUND_BLASTER_16_H
#define _SOUND_BLASTER_16_H

#include <stdint.h>

namespace SB16
{
    // DSP Ports
    #define DSP_MIXER_PORT          0x224
    #define DSP_MIXER_DATA_PORT     0x225
    #define DSP_RESET_PORT          0x226
    #define DSP_READ_PORT           0x22A
    #define DSP_WRITE_PORT          0x22C
    #define DSP_READ_STATUS_PORT    0x22E
    #define DSP_16_BIT_ACK_PORT     0x22F

    // DSP write commands
    #define DSP_SET_TIME_CONSTANT   0x40
    #define DSP_SET_OUT_SAMPLE_RATE 0x41
    #define DSP_SPEAKER_ON          0xD1
    #define DSP_SPEAKER_OFF         0xD3
    #define DSP_STOP_8_BIT_CHNL     0xD0
    #define DSP_RESUME_8_BIT_CHNL   0xD4
    #define DSP_STOP_16_BIT_CHNL    0xD5
    #define DSP_RESUME_16_BIT_CHNL  0xD6
    #define DSP_GET_DSP_VERSION     0xE1

    // DSP mixer port commands
    #define DSP_MIXER_MASTER_VOLUME 0x22
    #define DSP_MIXER_SET_IRQ       0x80

    void ResetDSP();
    void SetTransferMode(uint8_t mode);
    void SetSoundType(uint8_t type);
    void PlaySound();
}

#endif // _SOUND_BLASTER_16_H