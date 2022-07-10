#pragma once
#ifndef _ACPI_H
#define _ACPI_H

#include <types.h>

namespace ACPI
{
    struct RSDP2 {
        unsigned char Signature[8];
        uint8         Checksum;
        uint8         OEMID[6];
        uint8         Revision;
        uint32        RSDTAddress;
        uint32        Length;
        uint64        XSDTAddress;
        uint8         ExtendedChecksum;
        uint8         Reserved[3];
    } __attribute__((packed));

    struct XSDTHeader {
        unsigned char Signature[4];
        uint32        Length;
        uint8         Revision;
        uint8         Checksum;
        uint8         OEMID[6];
        uint8         OEMTableID[8];
        uint32        OEMRevision;
        uint32        CreatorID;
        uint32        CreatorRevision;
    } __attribute__((packed));

    struct MCFGHeader : XSDTHeader {
        uint64      Reserved;
    } __attribute__((packed));

    struct DeviceConfig {
        uint64  BaseAddress;
        uint16  PCISegGroup;
        uint8   StartBus;
        uint8   EndBus;
        uint32  Reserved;
    } __attribute__((packed));

    struct RecordHeader {
        uint8 EntryType;
        uint8 Length;
    };

    struct MADTHeader : XSDTHeader {
        uint32  LocalAPICAddress;
        uint32  Flags;
    } __attribute__((packed));

    void* FindTable(XSDTHeader* XSDTHeader, char* signature);
}

#endif // _ACPI_H