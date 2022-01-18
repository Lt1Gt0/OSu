#include "kbScancodeTranslation.h"

namespace QWERTYKeyboard{
    const char ASCIITable[] = {
        0x0, 0x0, '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=', 0x0, 0x0,
        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
        0x0, 0x0, 'a', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'', '`', 0x0, '\\',
        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/', 0x0, '*',
        0x0, ' '
    };

    char Translate(uint8_t scancode, bool uppercase){
        if(scancode > 58) return 0; // longer than array
        
        if(uppercase) 
          return ASCIITable[scancode] - 32;
        
        return ASCIITable[scancode];
    }
}
