#ifndef _AC97_H
#define _AC97_H

#include <stdint.h>

namespace AC97
{
    struct NativeAudioMixer {
        uint8_t     Reset;

        /**
            (default is 0x8000)
            5:0 (RW) Right Channel Volume (number * 1.5 = volume in db)
            7:6 (RO) Always 0
            13:8 (RW) Left Channel Volume (number * 1.5 = volume in db)
            14 (RO) Always 0
            15 (RW) Mute (1 = Mute Sound)
        */
        uint16_t    MasterOutputVolume;
        
        /**
            (Default is 0x8008)
            4:0 (RW) Microphone gain (every step is 1.5db)
            5 (RO) Always 0
            6 (RW) 20 db (1 = Add 20 db of volume (gain))
            01000b = 0 db, 00000b = 12 db, 11111b = -34.5 db
            14:7 (RO) Always 0
            15 (RW) Mute (1 = mute sound)
        */
        uint16_t    MicrophoneVolume;
        
        /**
            default is 0x8808)
            4:0 (RW) Right channel volume (every step is 1.5db)
            7:5 (RO) Always 0
            12:8 (RW) Left Channel Volume (every step is 1.5db)
            14:13 (RO) Always 0
            15 (RW) Mute (1 = mute sound)
        */
        uint16_t    PCMVolumeOutput;

        uint8_t     InputDevice;

        /**
            (Default is 0x8000)
            3:0 (RW) Right channel gain (number * 1.5 = Volume in db)
            7:4 (RO) Always 0
            11:8 (RW) Left channel gain (number * 1.5 = Vlue in db)
            14:12 (RO) Always 0
                15 (RW) Mute (1 = Mute Sound)
        */
        uint16_t    MicrophoneRecordGain;
    };

    struct NativeAudioBusMaster {

    };
}


#endif // _AC97_H