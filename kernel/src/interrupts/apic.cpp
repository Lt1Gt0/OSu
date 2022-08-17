#include <interrupts/apic.h>
#include <renderer.h>
#include <interrupts/apicEntries.h>
#include <interrupts/lapic.h>
#include <interrupts/ioapic.h>
#include <acpi.h>
#include <interrupts/pic.h>
#include <print.h>

namespace APIC
{
    LAPIC::LAPIC_REGS* lapic = nullptr;
    
    Record::ProcLAPIC* lapicRecords[0xFF];
    
    Record::IOAPIC* ioapicRecords[0xFF];
	IOAPIC ioapics[0xFF];
    uint16 ioapicCount;

    Record::IOAPIC_ISO* ioapicISORecords[0xFF];
    Record::ProcLocalx2APIC* Lx2APICRecords[0xFF];

    uint16 lapicProcEnabledCount;
    uint16 lapicOnlineCapableCount;

    uint16 lapicRecordCount;
    uint16 ioapicRecordCount;
    uint16 ioapicISORecordCount;
    uint16 Lx2APICRecordCount;
    
    void Initialize()
    {
        // Map Local APIC
        PageTableManager::MapMemory(lapic, lapic);
    }

    void ParseMADT(ACPI::MADTHeader* madt)
    {
        uint64 maxAddr = madt->Length + (uint64)madt;
        Record::Header* currentRecord = (Record::Header*)((uint64)madt + sizeof(ACPI::MADTHeader));
        uint64 lAPICPhysAddr = (uint64)(madt->LocalAPICAddress);

        if (madt->Flags & ACPI::MADT_PCAT_COMPAT_FLAG) 
            kprintf("MADT PC-AT Compatable\n");

        lapicRecordCount = 0;
        ioapicRecordCount = 0;
        ioapicCount = 0;
        ioapicISORecordCount = 0;
        Lx2APICRecordCount = 0;

        while ((uint64)currentRecord < maxAddr) {
            switch (currentRecord->Type) {
                case RecordType::ProcLAPIC:
                {
                    Record::ProcLAPIC* lapic = (Record::ProcLAPIC*)currentRecord;
                    
                    if (lapic->Flags & LAPIC_PROC_ENABLED_FLAG)
                        lapicProcEnabledCount++;
                    else if (lapic->Flags & LAPIC_ONLINE_CAPABLE_FLAG)
                        lapicOnlineCapableCount++;

                    lapicRecords[lapicRecordCount++] = lapic;
                    break;
                }
                case RecordType::IOAPIC:
                {
                    Record::IOAPIC* ioapic = (Record::IOAPIC*)currentRecord;
                    RegisterIOAPIC(ioapic->ID, ioapic->Address, ioapic->GlobalSystemInterruptBase);
                    break;
                }
                case RecordType::IOAPIC_ISO:
                {
                    Record::IOAPIC_ISO* ioapicISO = (Record::IOAPIC_ISO*)currentRecord;
                    ioapicISORecords[ioapicISO->IRQSource] = ioapicISO;
                    ioapicISORecordCount++;
                    break;
                }
                case RecordType::IOAPIC_NMISource:
                {
                    Record::IOAPIC_NMISource* ioapicNMISource = (Record::IOAPIC_NMISource*)currentRecord;
                    break;
                }
                case RecordType::LAPIC_NMI:
                {
                    Record::LAPIC_NMI* lapicNMI = (Record::LAPIC_NMI*)currentRecord;
                    break;
                }
                case RecordType::LAPIC_AddressOveride:
                {
                    Record::LAPIC_AddressOveride* lapicAddrOverride = (Record::LAPIC_AddressOveride*)currentRecord;
                    lAPICPhysAddr = lapicAddrOverride->PhysicalAddress;
                    break;
                }
                case RecordType::ProcLocalx2APIC:
                {
                    Record::ProcLocalx2APIC* lx2apic = (Record::ProcLocalx2APIC*)currentRecord;
                    Lx2APICRecords[Lx2APICRecordCount++] = lx2apic;
                    break;
                }
            }
            // Get the next record in the madt by adding the length of the current record to its address
            currentRecord = (Record::Header*)((uint64)currentRecord + currentRecord->Length);
        }
        lapic = (LAPIC::LAPIC_REGS*)lAPICPhysAddr;
    }

    void RegisterIOAPIC(uint8 id, uint32 addr, uint32 gsiBase)
    {
		IOAPIC* ioapic = nullptr;	
		// ioapics[id].ID = id;
        // ioapics[id].used = 1;
        // ioapics[id].gsiBase = gsiBase;
        // ioapics[id].physAddr = addr;
        // ioapics[id].addr = addr;

        // uint32 version = IOAPIC::ReadReg(addr, IOAPIC::regs::VERSION);
        // ioapics[id].maxRentry = (version >> 16) & 0xFF;
        // ioapics[id].version = version & 0xFF;
    
        // ioapicCount++;
    }
}
