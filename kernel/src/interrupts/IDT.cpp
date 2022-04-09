#include "interrupts/IDT.h"

void IDTDescEntry::SetOffset(uint64_t offset)
{
    offset0 = (uint16_t)(offset & 0x000000000000ffff);         // Lower 16 bits of the offset
    offset1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16); // Middle 16 bits of the offset
    offset2 = (uint32_t)((offset & 0xffffffff00000000) >> 32); // Upper 32 bits of the offset 
}

uint64_t IDTDescEntry::GetOffset()
{
    uint64_t offset = 0; 
    offset |= (uint64_t)offset0;
    offset |= (uint64_t)offset1 << 16;
    offset |= (uint64_t)offset2 << 32;
    return offset;
}
