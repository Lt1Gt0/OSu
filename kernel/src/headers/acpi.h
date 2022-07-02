#pragma once
#ifndef _ACPI_H
#define _ACPI_H

#include <stdint.h>

namespace ACPI
{
    struct RSDP2 {
        unsigned char   Signature[8];
        uint8_t         Checksum;
        uint8_t         OEMID[6];
        uint8_t         Revision;
        uint32_t        RSDTAddress;
        uint32_t        Length;
        uint64_t        XSDTAddress;
        uint8_t         ExtendedChecksum;
        uint8_t         Reserved[3];
    } __attribute__((packed));

    struct XSDTHeader {
        unsigned char   Signature[4];
        uint32_t        Length;
        uint8_t         Revision;
        uint8_t         Checksum;
        uint8_t         OEMID[6];
        uint8_t         OEMTableID[8];
        uint32_t        OEMRevision;
        uint32_t        CreatorID;
        uint32_t        CreatorRevision;
    } __attribute__((packed));

    struct MCFGHeader {
        XSDTHeader       Header;
        uint64_t        Reserved;
    } __attribute__((packed));

    struct DeviceConfig {
        uint64_t        BaseAddress;
        uint16_t        PCISegGroup;
        uint8_t         StartBus;
        uint8_t         EndBus;
        uint32_t        Reserved;
    } __attribute__((packed));

    struct RecordHeader {
        uint8_t EntryType;
        uint8_t Length;
    };

    struct MADTHeader {
        XSDTHeader  Header;
        uint32_t    LocalAPICAddress;
        uint32_t    Flags;
    } __attribute__((packed));

    void* FindTable(XSDTHeader* XSDTHeader, char* signature);
}

#endif // _ACPI_H