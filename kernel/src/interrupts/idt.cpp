#include <interrupts/idt.h>

void IDTDescEntry::SetOffset(uint64 offset)
{
    offset0 = (uint16)(offset & 0x000000000000ffff);         // Lower 16 bits of the offset
    offset1 = (uint16)((offset & 0x00000000ffff0000) >> 16); // Middle 16 bits of the offset
    offset2 = (uint32)((offset & 0xffffffff00000000) >> 32); // Upper 32 bits of the offset 
}

uint64 IDTDescEntry::GetOffset()
{
    uint64 offset = 0; 
    offset |= (uint64)offset0;
    offset |= (uint64)offset1 << 16;
    offset |= (uint64)offset2 << 32;
    return offset;
}
