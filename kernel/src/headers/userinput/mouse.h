#pragma once
#ifndef _MOUSE_H
#define _MOUSE_H

#include <types.h>
#include <IO.h>
#include <math.h>
#include <renderer.h>

/* IO PORTS FOR MOUSE COMM 
 * 0x60 (after Initialize of mouse) -  
 */

#define PS2LeftButton   0b00000001
#define PS2RightButton  0b00000010
#define PS2MiddleButton 0b00000100
#define PS2XSign        0b00010000
#define PS2YSign        0b00100000
#define PS2XOverflow    0b01000000
#define PS2YOverflow    0b10000000

/**
 * @brief Initialize the PS2 Mouse
 */
void InitPS2Mouse();

/*
 * @brief Handle PS2 Mouse incoming packets
 *
 * @param data
 */
void HandlePS2Mouse(uint8 data);

/**
 * @brief Process a PS2 mouse packet
 */
void ProcessMousePacket();

extern uint8 MousePointer[];
extern Point MousePosition;

#endif // _MOUSE_H
