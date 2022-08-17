#pragma once
#ifndef _PAGE_TABLE_MANAGER_H
#define _PAGE_TABLE_MANAGER_H

#include <paging/paging.h>

namespace PageTableManager
{
    void PageTableManager(PageTable* PML4Address);
    
    /**
     * @brief Map a physical memory address to a virtual memory address
     * 
     * @param virtualMemory 
     * @param physicalMemory 
     */
    void MapMemory(void* virtualMemory, void* physicalMemory);
};

#endif // _PAGE_TABLE_MANAGER_H