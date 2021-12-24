#pragma once
#include "Typedefs.h"

namespace Keyboard{
    enum SpecialScancodes{
        BACKSPACE_PRESSED = 0x0E,
        BACKSPACE_RELEASED = 0x8E,
        L_SHIFT_PRESSED = 0x2A,
        L_SHIFT_RELEASED = 0xAA,
        R_SHIFT_PRESSED = 0x36,
        R_SHIFT_RELEASED = 0xB6,
        ARROW_DOWN = 0x50,
        ARROW_UP = 0x48,
        PAGE_UP_PRESSED = 0x49,
        PAGE_UP_RELEASED = 0xC9,
        PAGE_DOWN_PRESSED = 0x7A,
        PAGE_DOWN_RELEASED = 0xD1,
        HOME_PRESSED = 0x47, 
        HOME_RELEASED = 0xC7, 
        END_PRESSED =  0x4F,
        END_RELEASED =  0xCF,

        
        ENTER_PRESSED = 0x9C
    };

    
    /**
     * @brief 
     * 
     * @param scanCode 
     * @param chr 
     */
    void standardKBHandler(byte scanCode, byte chr);

    /**
     * @brief 
     * 
     * @param scanCode 
     * @param chr 
     */
    void specialKbHandler(byte scanCode, byte chr);

    /**
     * @brief 
     * 
     */
    void kbHandler0xE0(byte scanCode);
}