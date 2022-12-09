#ifndef _AC97_H
#define _AC97_H

#include <stdint.h>

namespace AC97
{
    enum Revision : uint8_t {

    };

    enum NativeAudioMixerRegister : uint8_t {
        Reset                       = 0x00,
        SetMasterOutputVolume       = 0x02,
        SetAuxOutputVolume          = 0x04,
        SetMonoVolume               = 0x06,
        SetMasterTone               = 0x08,
        SetPCBeepVolume             = 0x0A,
        SetPhoneVolume              = 0x0C,
        SetMicrophoneVolume         = 0x0E,
        LineInVolume                = 0x10,
        SetCDVolume                 = 0x12,
        SetVideoVolume              = 0x14,
        AuxInVolume                 = 0x16,
        PCMOutVolume                = 0x18,
        RecordSelect                = 0x1A,
        RecordGain                  = 0x1C,
        RecordGainMic               = 0x1E,
        GeneralPurpose              = 0x20,
        Control3D                   = 0x22,
        PowerdownCtrl_Stat          = 0x26,
        ExtendedAudioID             = 0x28,
        ExtendedAudioStatusControl  = 0x2A,
        PCMFrontDACRate             = 0x2C,
        PCMSurrDACRate              = 0x2E,
        PCMLFEDACRate               = 0x30,
        PCMLRADCRate                = 0x32,
        MicADCRate                  = 0x32,
        CenterLFEVolume             = 0x36,
        SurrVolume                  = 0x38,
        S_PDIFControl               = 0x3A,
        ExtendedModem               = 0x3C,
        ExtdModemStat_Ctrl          = 0x3E,
        Line1DAC_ADCRate            = 0x40,
        Line2DAC_ADCRate            = 0x42,
        HandsetDAC_ADCRate          = 0x44,
        Line1DAC_ADCLevel           = 0x46,
        Line2DAC_ADCLevel           = 0x48,
        HandsetDAC_ADCLevel         = 0x4A,
        GPIOPinConfig               = 0x4C,
        GPIOPinPolarity_Type        = 0x4E,
        GPIOPinSticky               = 0x50,
        GPIOPinWakeup               = 0x52,
        GPIOPinStatus               = 0x54,
        MiscMdmAFEStat_Ctrl         = 0x56,
        VendorID1                   = 0x7C,
        VencodeID2                  = 0x7E
    };

    enum NativeAudioBusChannel : uint8_t {
        PCMIn           = 0x00,
        PCMOut          = 0x10,
        MicrophoneIn    = 0x20,
        Microphone2     = 0x40,
        PCMIn2          = 0x50,
        SPDIF           = 0x60
    };

    enum NativeAudioBusRegister : uint8_t {
        GlobalControl = 0x2C
    };

    void PlaySound();
    void StopSound();
    void ResumeSound();

    void ColdReset();
    void WarmReset();
    void RegisterReset();
}


#endif // _AC97_H