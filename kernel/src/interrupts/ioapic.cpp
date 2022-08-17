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
        //// this->m_virtualAddr

        //this->m_virtualAddr += this->m_physRegs % PAGE_SIZE;

        //this->m_apicID = ((read(IOAPICID) >> 24)) & 0xF0;
        //this->m_apicVersion = read(IOAPICVER);

        //this->m_redirectionEntryCount = (read(IOAPICVER) >> 16) + 1;
        //this->m_globalInterruptBase = gsib;
    //}

    uint8 IOAPIC::id()
    {
        return this->m_apicID;
    }

    uint8 IOAPIC::version()
    {
        return this->m_apicVersion;
    }

    uint8 IOAPIC::redirectionEntries()
    {
        return this->m_redirectionEntryCount;
    }

    size_t IOAPIC::globalInterruptBase()
    {
        return this->m_globalInterruptBase;
    }

    IOAPIC::RedirectionEntry IOAPIC::getRedirectionEntry(uint8 entryNum)
    {

    }

    void IOAPIC::writeRedirectionEntry(uint8 entryNum, IOAPIC::RedirectionEntry* entry)
    {

    }

    uint32 IOAPIC::read(uint8 regOff)
    {
        *(uint32 volatile*) m_virtualAddr = regOff;
        return *(uint32 volatile*)(m_virtualAddr + 0x10);
    }

    void IOAPIC::write(uint8 regOff, uint32 data)
    {
        *(uint32 volatile*) m_virtualAddr = regOff;
        *(uint32 volatile*)(m_virtualAddr + 0x10) = data;
    }
}
