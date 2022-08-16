#pragma once
#ifndef _BOOT_STRING_H
#define _BOOT_STRING_H

#include <efi.h>
#include "common.h"
#include "efibind.h"

UINTN memcmp
(
		IN const VOID*	aptr,
	   	IN const VOID*	bptr,
	   	IN size_t		n
);

UINTN strcmp
(
		IN CHAR8*	a,
	   	IN CHAR8*	b,
	   	IN UINTN	len
);

#endif // _BOOT_STRING_H
