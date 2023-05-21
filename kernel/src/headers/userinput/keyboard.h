#pragma once
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <types.h>
#include <userinput/kbScancodeTranslation.h>

void HandlerKeyboardSpecial(uint8 scancode);
void HandlerKeyboard(uint8 scancode);

#endif // _KEYBOARD_H_
