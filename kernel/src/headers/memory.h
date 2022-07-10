#pragma once
#ifndef _MEMORY_H
#define _MEMORY_H

#include <types.h>
#include <efiMemory.h>

uint64 GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64 mMapEntries, uint64 mMapDescSize);
void memset(void* start, uint8 value, uint64 num);

#endif // _MEMORY_H