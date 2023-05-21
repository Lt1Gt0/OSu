#include <paging/pagetableManager.h>
#include <types.h>
#include <paging/pagemapIndexer.h>
#include <paging/pageframeAllocator.h>
#include <memory/memory.h>

namespace PageTableManager
{
    PageTable* PML4;

    void PageTableManager(PageTable* PML4Address)
    {
        PML4 = PML4Address;
    }

    void MapMemory(void* virtualMemory, void* physicalMemory)
    {
        PageMapIndexer indexer = PageMapIndexer((uint64)virtualMemory);
        PageDirectoryEntry PDE;

        PDE = PML4->entries[indexer.mPDPidx];
        PageTable* PDP;

        // Create new PDP
        if (!PDE.GetFlag(PT_Flag::Present)) {
            PDP = (PageTable*)GlobalAllocator.RequestPage();
            memset(PDP, 0, PAGE_SIZE);
            PDE.SetAddress((uint64)PDP >> 12);
            PDE.SetFlag(PT_Flag::Present, true);
            PDE.SetFlag(PT_Flag::ReadWrite, true);
            PML4->entries[indexer.mPDPidx] = PDE;
        } else {
            PDP = (PageTable*)((uint64)PDE.GetAddress() << 12); // Get the address stored in the PDP
        }

        PDE = PDP->entries[indexer.mPDidx];
        PageTable* PD;

        // Create new PD
        if (!PDE.GetFlag(PT_Flag::Present)) {
            PD = (PageTable*)GlobalAllocator.RequestPage();
            memset(PD, 0, PAGE_SIZE);
            PDE.SetAddress((uint64)PD >> 12);
            PDE.SetFlag(PT_Flag::Present, true);
            PDE.SetFlag(PT_Flag::ReadWrite, true);
            PDP->entries[indexer.mPDidx] = PDE;
        } else {
            PD = (PageTable*)((uint64)PDE.GetAddress() << 12); // Get the address stored in the PDP
        }

        PDE = PD->entries[indexer.mPTidx];
        PageTable* PT;

        // Create new PT
        if (!PDE.GetFlag(PT_Flag::Present)) {
            PT = (PageTable*)GlobalAllocator.RequestPage();
            memset(PT, 0, PAGE_SIZE);
            PDE.SetAddress((uint64)PT >> 12);
            PDE.SetFlag(PT_Flag::Present, true);
            PDE.SetFlag(PT_Flag::ReadWrite, true);
            PD->entries[indexer.mPTidx] = PDE;
        } else {
            PT = (PageTable*)((uint64)PDE.GetAddress() << 12); // Get the address stored in the PDP
        }

        PDE = PT->entries[indexer.mPidx];
        PDE.SetAddress((uint64)physicalMemory >> 12); // Convert physical memory into an accessible address for the PDP
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PT->entries[indexer.mPidx] = PDE;
    }
}
