#pragma once
#include <stdint.h>

struct PageDirectoryEntry{
    bool Present        : 1; // Page is present meaning the MMU can access it
    bool ReadWrite      : 1; // Page can be read or written to
    bool UserSuper      : 1; // Privilage level
    bool WriteThrough   : 1;
    bool CacheDisabled  : 1;
    bool Accessed       : 1;
    bool ignore0        : 1; // Reserved
    bool LargerPages    : 1;
    bool ignore1        : 1; // Reserved
    uint8_t Available   : 3;
    uint64_t Address    : 52;
};

struct PageTable {
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000))); // align the page table