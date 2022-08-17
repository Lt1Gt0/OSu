#pragma once
#ifndef _PAGE_MAP_INDEXER_H
#define _PAGE_MAP_INDEXER_H

#include <types.h>

class PageMapIndexer
{
    public:
        PageMapIndexer(uint64 virtualAddress);
        uint64 PDP_i;
        uint64 PD_i;
        uint64 PT_i;
        uint64 P_i;
};

#endif // _PAGE_MAP_INDEXER_H