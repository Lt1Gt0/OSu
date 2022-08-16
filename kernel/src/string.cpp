#include <string.h>
#include <renderer.h>

char uinttostrOutput[128]; // Buffer to hold characters in
const char* tostr(uint64 value)
{
    uint8 size; // Size of the string that will be produced
    uint64 sizeTest = value;
    
    //Get the size of the string
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }

    uint8 index = 0; // Buffer index
    while (value / 10 > 0) {
        uint8 remainder = value % 10;
        value /= 10;
        uinttostrOutput[size - index] = remainder + '0'; // Working backwards
        index++;
    }

    uint8 remainder = value % 10; // Finalize value
    uinttostrOutput[size - index] = remainder + '0';
    uinttostrOutput[size + 1] = 0; // Null termination
    return uinttostrOutput;
}

char inttostrOutput[128]; // Buffer to hold characters in
const char* tostr(int64 value)
{
    uint8 isNegative = 0;
    if (value < 0) {
        isNegative = 1;
        value *= -1;
        inttostrOutput[0] = '-';
    }

    uint8 size; // Size of the string that will be produced
    uint64 sizeTest = value;
    
    //Get the size of the string
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }

    uint8 index = 0; // Buffer index
    while (value / 10 > 0) {
        uint8 remainder = value % 10;
        value /= 10;
        inttostrOutput[isNegative + size - index] = remainder + '0'; // Working backwards
        index++;
    }

    uint8 remainder = value % 10; // Finalize value
    inttostrOutput[isNegative + size - index] = remainder + '0';
    inttostrOutput[isNegative + size + 1] = 0; // Null termination
    return inttostrOutput;
}

char hextostrOutput64[128];
const char* tohstr(uint64 value)
{
    uint64* valPtr = &value;
    uint8* ptr;
    uint8 tmp;
    uint8 size = 8 * 2 - 1;
    for (uint8 i = 0; i < size; i++) {
        ptr = ((uint8*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hextostrOutput64[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hextostrOutput64[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hextostrOutput64[size + 1] = 0; //Null termination
    return hextostrOutput64;
}

char hextostrOutput32[128];
const char* tohstr(uint32 value)
{
    uint32* valPtr = &value;
    uint8* ptr;
    uint8 tmp;
    uint8 size = 4 * 2 - 1;
    for (uint8 i = 0; i < size; i++) {
        ptr = ((uint8*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hextostrOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hextostrOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hextostrOutput32[size + 1] = 0; //Null termination
    return hextostrOutput32;
}

char hextostrOutput16[128];
const char* tohstr(uint16 value)
{
    uint16* valPtr = &value;
    uint8* ptr;
    uint8 tmp;
    uint8 size = 2 * 2 - 1;
    for (uint8 i = 0; i < size; i++) {
        ptr = ((uint8*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hextostrOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hextostrOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hextostrOutput16[size + 1] = 0; //Null termination
    return hextostrOutput16;
}

char hextostrOutput8[128];
const char* tohstr(uint8 value)
{
    uint8* valPtr = &value;
    uint8* ptr;
    uint8 tmp;
    uint8 size = 1 * 2 - 1;
    for (uint8 i = 0; i < size; i++) {
        ptr = ((uint8*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hextostrOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
        tmp = ((*ptr & 0x0F));
        hextostrOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0'); // ASCII representation for hex
    }

    hextostrOutput8[size + 1] = 0; //Null termination
    return hextostrOutput8;
}

char doubletostrOutput[128];
const char* tostr(double value, uint8 decimalPlaces)
{
    if (decimalPlaces > 20)
        decimalPlaces = 20;

    char* intPtr = (char*)tostr((int64)value);
    char* doublePtr = doubletostrOutput;

    if (value < 0)
        value *= -1;

    while (*intPtr != 0) {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newVal = value - (int)value; // Take out the decimal portion

    for (uint8 i = 0; i < decimalPlaces; i++) {
        newVal *= 10;
        *doublePtr = (int)newVal + '0';
        newVal -= (int)newVal;
        doublePtr++;
    }

    //Finialize string
    *doublePtr = 0;
    return doubletostrOutput;
}

const char* tostr(double value)
{
    return tostr(value, 2);
}

int memcmp(const void* aptr, const void* bptr, size_t size)
{
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;

    for (int i = 0; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1; 
    }

    return 0;
}

void* memset(void* bufptr, int val, size_t size)
{
    unsigned char* buf = (unsigned char*)bufptr;
    for (size_t i = 0; i < size; i++) {
        buf[i] = (unsigned char)val;
    }

    return bufptr;
}

size_t strlen(const char* str)
{
    size_t size = 0;

    while (*str++) {
        size++;
    }

    return size;
}

void* memcpy(void* dstptr, const void* srcptr, size_t size)
{
    unsigned char* dst = (unsigned char*)dstptr;
    const unsigned char* src = (const unsigned char*)srcptr;
    for (size_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }

    return dstptr;
}

char fmtBuf[0x1000];
#pragma region const char* format(char* fstring, i64...)
__attribute__((always_inline)) inline void finsert(nint value, char pformat, nint& bufferIndexer)
{
	// the string that we are inserting into the fbuffer
	char* str = nullptr;

	// Convert value to string based on requested format
	switch (pformat)
	{
		// Basic signed integer
		case 0x00:
			str = (char*)tostr(value, true);
			break;

		// Hex string
		case 'x':
		case 'X':
			// str = (char*)tohex(value);
			break;

		// Unsigned integer
		case 'u':
		case 'U':
			str = (char*)tostr(value, false);
			break;

		// String pointer
		case 's':
		case 'S':
			str = (char*)value;
			break;

		default:
			bufferIndexer += 3;
			return;
	}

	if (!str) return;

	nint len = strlen(str);
	char* fbase = &fmtBuf[bufferIndexer];
	memcpy(fbase, str, len);
	bufferIndexer += len;
}

const char* format(const char* fmt, nint p0, nint p1, nint p2, nint p3, nint p4, nint p5, nint p6, nint p7, nint p8, nint p9)
{
	// the length of the base fstring
	nint fstrlen = strlen(fmt);

	// the current index in the fstring
	nint fstrIndexer = 0;

	// the current index in the buffer
	nint bufferIndexer = 0;

	// a lookup table for the parameters
	nint lookup[10] = {
		p0, p1, p2,
		p3, p4, p5,
		p6, p7, p8,
			p9
	};

	while (fstrIndexer != fstrlen)
	{
		switch (fmt[fstrIndexer])
		{
			case '%':
			{
				char nextChar = fmt[fstrIndexer + 1];
				switch (nextChar)
				{
					// Basic signed integer
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						finsert(lookup[nextChar - '0'], 0x00, bufferIndexer);
						fstrIndexer += 2;
						break;

					// Unsigned hex string
					case 'x':
					case 'X':
					{
						nextChar = fmt[fstrIndexer + 2];
						switch (nextChar)
						{
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								finsert(lookup[nextChar - '0'], 'x', bufferIndexer);
								fstrIndexer += 3;
								break;

							case 0x00:
							default:
								fmtBuf[bufferIndexer++] = fmt[fstrIndexer++];
								break;
						}
					}
					break;

					// Unsigned integer
					case 'u':
					case 'U':
					{
						nextChar = fmt[fstrIndexer + 2];
						switch (nextChar)
						{
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								finsert(lookup[nextChar - '0'], 'u', bufferIndexer);
								fstrIndexer += 3;
								break;

							case 0x00:
							default:
								fmtBuf[bufferIndexer++] = fmt[fstrIndexer++];
								break;
						}
					}
					break;

					// String
					case 's':
					case 'S':
					{
						nextChar = fmt[fstrIndexer + 2];
						switch (nextChar)
						{
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								finsert(lookup[nextChar - '0'], 's', bufferIndexer);
								fstrIndexer += 3;
								break;

							case 0x00:
							default:
								fmtBuf[bufferIndexer++] = fmt[fstrIndexer++];
								break;
						}
					}
					break;

					case 0:
					default:
						fmtBuf[bufferIndexer++] = fmt[fstrIndexer++];
						break;
				}
				break;
			}

			default:
				fmtBuf[bufferIndexer++] = fmt[fstrIndexer++];
				break;
		}
	}

	// null terminate
    fmtBuf[bufferIndexer] = 0x00;
	return fmtBuf;
}
