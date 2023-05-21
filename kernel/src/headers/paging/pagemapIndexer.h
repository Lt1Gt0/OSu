#pragma once
#ifndef _PAGE_MAP_INDEXER_H_
#define _PAGE_MAP_INDEXER_H_

#include <types.h>

class PageMapIndexer
{
    public:
        PageMapIndexer(uint64 virtualAddress);
        uint64 mPDPidx;
        uint64 mPDidx;
        uint64 mPTidx;
        uint64 mPidx;
};

#endif // _PAGE_MAP_INDEXER_H_
