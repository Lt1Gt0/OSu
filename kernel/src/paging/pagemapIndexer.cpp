#include <paging/pagemapIndexer.h>

PageMapIndexer::PageMapIndexer(uint64 virtualAddress)
{
    virtualAddress >>= 12; // Each address in the pages are aligned by 12 bits
    this->mPidx = virtualAddress & 0x1FF; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    this->mPTidx = virtualAddress & 0x1FF; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    this->mPDidx = virtualAddress & 0x1FF; // Part of the virtual address that corresponds to index
    virtualAddress >>= 9;
    this->mPDPidx = virtualAddress & 0x1FF; // Part of the virtual address that corresponds to index
}
