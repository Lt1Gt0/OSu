#pragma once
#ifndef _APIC_H
#define _APIC_H

#include <stdint.h>

namespace APIC
{
    static uint8_t lApicIds[256];
    static uint8_t numcore;
    static uint64_t lApicPtr;
    static uint64_t ioApicPtr;

    void DetectCores(uint8_t* rsdt);
    void LoadHeader();
}

#endif // _APIC_H