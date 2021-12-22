#pragma once
#include "Typedefs.h"

namespace Keyboard{
    bool leftShiftPressed = false;
    bool rightShiftPressed = false;
    byte lastScanCode;
    
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