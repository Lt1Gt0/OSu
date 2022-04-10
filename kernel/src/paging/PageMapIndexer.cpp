#include <paging/PageMapIndexer.h>

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress)
{
    virtualAddress >>= 12; // Each address in the pages are aligned by 12 bits
    P_i = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    PT_i = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    PD_i = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    PDP_i = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
}