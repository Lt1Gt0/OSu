#include "bootloader/string.h"

int memcmp(const void *aptr, const void *bptr, size_t n)
{
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++) {
		if (a[i] < b[i])
			return -1;
			
		else if (a[i] > b[i])
			return 1;
	}

	return 0;
}

UINTN strcmp(CHAR8 *a, CHAR8 *b, UINTN len)
{
	for (UINTN i = 0; i < len; i++) {
		if (*a != *b)
			return 0;

		a++;
		b++;
	}
	return 1;
}
