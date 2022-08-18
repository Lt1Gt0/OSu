#pragma once
#ifndef _PAGE_MAP_INDEXER_H
#define _PAGE_MAP_INDEXER_H

#include <types.h>

class PageMapIndexer
{
    public:
        PageMapIndexer(uint64 virtualAddress);
        uint64 mPDP_idx;
        uint64 mPD_idx;
        uint64 mPT_idx;
        uint64 mP_idx;
};

#endif // _PAGE_MAP_INDEXER_H
