#pragma once
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <types.h>
#include <userinput/kbScancodeTranslation.h>

void HandlerKeyboardSpecial(uint8 scancode);
void HandlerKeyboard(uint8 scancode);

#endif // _KEYBOARD_H