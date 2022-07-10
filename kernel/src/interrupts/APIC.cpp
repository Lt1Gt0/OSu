#include <interrupts/APIC.h>

#include <BasicRenderer.h>
#include <interrupts/APICEntries.h>
#include <interrupts/LAPIC.h>
#include <interrupts/IOAPIC.h>

namespace APIC
{
    LAPIC::LAPIC_REGS* lapic = nullptr;
    
    Record::ProcLAPIC* LAPICRecords[0xFF];
    Record::IOAPIC* IOAPICRecords[0xFF];
    Record::IOAPIC_ISO* IOAPICISORecords[0xFF];
    Record::ProcLocalx2APIC* Lx2APICRecords[0xFF];

    uint16 LAPICRecordCount;
    uint16 IOAPICRecordCount;
    uint16 IOAPICISORecordCount;
    uint16 Lx2APICRecordCount;

    void ParseMADT(ACPI::MADTHeader* madt)
    {
        uint64 maxAddr = madt->Length + (uint64)madt;
        Record::Header* currentRecord = (Record::Header*)((uint64)madt + sizeof(ACPI::MADTHeader));
        uint64 lAPICPhysAddr =  (uint64)(madt->LocalAPICAddress);

        LAPICRecordCount = 0;
        IOAPICRecordCount = 0;
        IOAPICISORecordCount = 0;
        Lx2APICRecordCount = 0;

        while ((uint64)currentRecord < maxAddr) {
            switch (currentRecord->Type) {
            case RecordType::ProcLAPIC:
                {
                    Record::ProcLAPIC* lapic = (Record::ProcLAPIC*)currentRecord;
                    LAPICRecords[LAPICRecordCount++] = lapic;
                    break;
                }
            case RecordType::IOAPIC:
                {
                    Record::IOAPIC* ioapic = (Record::IOAPIC*)currentRecord;
                    IOAPICRecords[IOAPICRecordCount++] = ioapic;
                    break;
                }
            case RecordType::IOAPIC_ISO:
                {
                    Record::IOAPIC_ISO* ioapicISO = (Record::IOAPIC_ISO*)currentRecord;
                    IOAPICISORecords[ioapicISO->IRQSource] = ioapicISO;
                    IOAPICISORecordCount++;
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
}