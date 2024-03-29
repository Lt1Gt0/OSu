#pragma once
#ifndef _IO_APIC_H_
#define _IO_APIC_H_

#include <interrupts/apicEntries.h>
#include <types.h>

namespace APIC
{
    constexpr u8 IOAPICID        {0x00}; 
    constexpr u8 IOAPICVER       {0x01};
    constexpr u8 IOAPICARB       {0x02};
    #define IOAPICREDTBL(n) (0x10 + 2 * n) // Lower 32-bits (add 1 for upper 32 bits)

    struct IOAPIC
    {
        public:
            enum DeliveryMode {
                EDGE = 0,
                LEVEL = 1,
            };

            enum DestinationMode {
                PHYSICAL = 0,
                LOGICAL = 1,
            };

            union RedirectionEntry {
                struct {
                    uint64 vector : 8;
                    uint64 deliveryMode : 3;
                    uint64 destinationMode : 1;
                    uint64 deliveryStatus : 1;
                    uint64 pinPolarity : 1;
                    uint64 remoteIRR : 1;
                    uint64 triggerMode : 1;
                    uint64 mask : 1;
                    uint64 RESV : 39;
                    uint64 destination : 8;
                };

                struct {
                    uint32 lowerDWORD;
                    uint32 upperDWORD;
                };
            };

            //IOAPIC(size_t physRegs, size_t apicID, size_t gsib);
            uint8 id();
            uint8 version();
            uint8 redirectionEntries();
            size_t globalInterruptBase();
            RedirectionEntry getRedirectionEntry(uint8 entryNum);
            void writeRedirectionEntry(uint8 entryNum, RedirectionEntry* entry);

        private:
            size_t mPhysRegs;
            size_t mVirtualAddr;
            size_t mGlobalInterruptBase;
            uint8 mApicID;
            uint8 mApicVersion;
            uint8 mRedirectionEntryCount;

            uint32 read(uint8 regOff);
            void write(uint8 regOff, uint32 data);
    };
}

#endif // _IO_APIC_H_
