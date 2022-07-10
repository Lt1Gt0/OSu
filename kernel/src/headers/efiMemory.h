#pragma once
#ifndef _EFI_MEMORY_H
#define _EFI_MEMORY_H

#include <types.h>

struct EFI_MEMORY_DESCRIPTOR {
    uint32  type;
    void*   physAddr;
    void*   virtAddr;
    uint64  numPages;
    uint64  attribs;
};

extern const char* EFI_MEMORYYPE_STRINGS[];

#endif // _EFI_MEMORY_H