#include <interrupts/ioapic.h>

#include <types.h>
#include <interrupts/apicEntries.h>
#include <acpi.h>
#include <paging/pageframeAllocator.h>

namespace APIC
{
	#define IOREGSEL 0x00
	#define IOREGWIN 0x10

    //IOAPIC::IOAPIC(size_t physRegs, size_t apicID, size_t gsib)
    //{
        //// m_virtualAddr

        //m_virtualAddr += m_physRegs % PAGE_SIZE;

        //m_apicID = ((read(IOAPICID) >> 24)) & 0xF0;
        //m_apicVersion = read(IOAPICVER);

        //m_redirectionEntryCount = (read(IOAPICVER) >> 16) + 1;
        //m_globalInterruptBase = gsib;
    //}

    uint8 IOAPIC::id()
    {
        return mApicID;
    }

    uint8 IOAPIC::version()
    {
        return mApicVersion;
    }

    uint8 IOAPIC::redirectionEntries()
    {
        return mRedirectionEntryCount;
    }

    size_t IOAPIC::globalInterruptBase()
    {
        return mGlobalInterruptBase;
    }

    IOAPIC::RedirectionEntry IOAPIC::getRedirectionEntry(uint8 entryNum)
    {

    }

    void IOAPIC::writeRedirectionEntry(uint8 entryNum, IOAPIC::RedirectionEntry* entry)
    {

    }

    uint32 IOAPIC::read(uint8 regOff)
    {
        *(uint32 volatile*) mVirtualAddr = regOff;
        return *(uint32 volatile*)(mVirtualAddr + 0x10);
    }

    void IOAPIC::write(uint8 regOff, uint32 data)
    {
        *(uint32 volatile*) mVirtualAddr = regOff;
        *(uint32 volatile*)(mVirtualAddr + 0x10) = data;
    }
}
