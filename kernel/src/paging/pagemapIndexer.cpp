#include <paging/pagemapIndexer.h>

PageMapIndexer::PageMapIndexer(uint64 virtualAddress)
{
    virtualAddress >>= 12; // Each address in the pages are aligned by 12 bits
    this->mP_idx = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    this->mPT_idx = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    this->mPD_idx = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    this->mPDP_idx = virtualAddress & 0x1ff; // Part of the virtual address that corresponds to index
}
