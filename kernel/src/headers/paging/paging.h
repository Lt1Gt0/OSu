#pragma once
#ifndef _PAGING_H
#define _PAGING_H

#define PAGESIZE 0x1000

#include <types.h>

enum PT_Flag {
    Present         = 0, // Page is present meaning the MMU can access it
    ReadWrite       = 1, // Page can be read or written to
    UserSuper       = 2, // Privilage level
    WriteThrough    = 3,
    CacheDisabled   = 4,
    Accessed        = 5,
    LargerPages     = 7,
    Custom0         = 9,
    Custom1         = 10,
    Custom2         = 11,
    NX              = 63 //Only if supported
};

struct PageDirectoryEntry {
    uint64  Value;
    void    SetFlag(PT_Flag flag, bool enabled);
    bool    GetFlag(PT_Flag flag);
    void    SetAddress(uint64 address);
    uint64  GetAddress();
};

struct PageTable {
    PageDirectoryEntry entries [512];
} __attribute__((aligned(PAGESIZE))); // align the page table

#endif // _PAGING_H