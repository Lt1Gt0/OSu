#pragma once
#ifndef _GDT_H
#define _GDT_H

#include <types.h>

struct GDTDescriptor {
     uint16 Size;
     uint64 Offset;
}__attribute__((packed));

struct GDTEntry {
     uint16 Limit0;
     uint16 Base0;
     uint8  Base1;
     uint8  AccessByte;
     uint8  Limit1_Flags;
     uint8  Base2;
}__attribute__((packed));

struct GDT {
    GDTEntry  Null;          // 0x00
    GDTEntry  KernelCode;    // 0x08
    GDTEntry  KernelData;    // 0x10
    GDTEntry  UserNull;      
    GDTEntry  UserCode;
    GDTEntry  UserData;
} __attribute__((packed)) 
__attribute__((aligned(0x1000)));

extern "C" GDT DefaultGDT;
extern "C" void LoadGDT(GDTDescriptor* gdtDescriptor);

#endif // _GDT_H