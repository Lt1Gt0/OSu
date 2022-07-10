#include <paging/paging.h>

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled)
{
    uint64 bitSelector = (uint64)1 << flag;
    Value &= ~bitSelector;
    if (enabled)
        Value |= bitSelector;
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag)
{
    uint64 bitSelector = (uint64)1 << flag;
    return Value & bitSelector > 0 ? true : false; 
}

uint64 PageDirectoryEntry::GetAddress()
{
    return (Value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64 address)
{
    address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff; //Clear address from middle of value
    Value |= (address << 12);
}
