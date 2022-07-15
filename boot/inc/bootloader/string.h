#ifndef _BOOT_STRING_H
#define _BOOT_STRING_H

#include <efi.h>
#include "common.h"

int memcmp(const void* aptr, const void* bptr, size_t n);
UINTN strcmp(CHAR8* a, CHAR8* b, UINTN len);

#endif // _BOOT_STRING_H
