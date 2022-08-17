#pragma once
#ifndef _IDT_H
#define _IDT_H

#include <types.h>

/*** Common Gates ***/
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_TrapGate         0b10001111

struct IDTDescEntry {
    uint16  offset0;
    uint16  selector;       // Segment selector
    uint8   ist;            // Interrupt stack table offset
    uint8   type_attr;		// Gate Type | 0 | DPL | P
    uint16  offset1;
    uint32  offset2;
    uint32  RESV;
    void    SetOffset(uint64 offset);
    uint64  GetOffset();
};

struct IDTR {
    uint16  Limit;
    uint64  Offset;
} PACKED;

#endif // _IDT_H
