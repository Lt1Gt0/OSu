#pragma once
#include <stdint.h>

enum PT_Flag{
    Present = 0, // Page is present meaning the MMU can access it
    ReadWrite = 1, // Page can be read or written to
    UserSuper = 2, // Privilage level
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 //Only if supported
};

struct PageDirectoryEntry{
    uint64_t Value;
    void SetFlag(PT_Flag flag, bool enabled);
    bool GetFlag(PT_Flag flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable {
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000))); // align the page table