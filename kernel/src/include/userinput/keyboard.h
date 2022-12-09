#pragma once
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdint.h>
#include <userinput/kbScancodeTranslation.h>
#include <BasicRenderer.h>
#include <globals.h>
void HandlerKeyboard(uint8_t scancode);

#endif // _KEYBOARD_H