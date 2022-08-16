#include "bootloader/string.h"

UINTN memcmp
(
		IN const VOID*	aptr,
	   	IN const VOID*	bptr,
	   	IN size_t		n
)
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

UINTN strcmp
(
		IN CHAR8*	a,
	   	IN CHAR8*	b,
	   	IN UINTN	len
)
{
	for (UINTN i = 0; i < len; i++) {
		if (*a != *b)
			return 0;

		a++;
		b++;
	}
	return 1;
}
