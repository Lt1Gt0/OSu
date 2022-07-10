#pragma once
#ifndef _IO_APIC_H
#define _IO_APIC_H

#include <types.h>

namespace APIC
{
    namespace DeliveryMode 
    {
        enum : uint8 {
            EDGE    = 0,
            LEVEL   = 1,
        };
    }

    namespace DestinationMode
    {
        enum : uint8 {
            PHYSICAL    = 0,
            LOGICAL     = 1,
        };
    }

    union RedirectionEntry
    {
        struct {
            uint64 vector             : 8;
            uint64 deliveryMode       : 3;
            uint64 destinationMode    : 1;
            uint64 deliveryStatus     : 1;
            uint64 pinPolarity        : 1;
            uint64 remoteIRR          : 1;
            uint64 triggerMode        : 1;
            uint64 mask               : 1;
            uint64 reserved           : 39;
            uint64 destination        : 8;
        };

        struct {
            uint32 lowerDWORD;
            uint32 upperDWORD;
        };
    };

    namespace IOAPIC 
    {
        void Configure();
    }
}

#endif // _IO_APIC_H