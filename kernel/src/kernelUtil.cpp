#include <kernelUtil.h>

#include <memory.h>
#include <paging/PageFrameAllocator.h>
#include <userinput/mouse.h>
#include <gdt/gdt.h>
#include <interrupts/IDT.h>
#include <interrupts/interrupts.h>
#include <interrupts/PIC.h>
#include <interrupts/APIC.h>
#include <timer/pit/pit.h>
#include <print.h>

void PrepareMemory(BootInfo* bootInfo)
{
    uint64 mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize; // get the total map entries by dividing the size of the map by the descriptor size

    GlobalAllocator = PageFrameAllocator(); // Declare the global allocator to an instance of a PageFrameAllocator
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    // Get the kernel size 
    uint64 kernelSize = (uint64)&_KernelEnd - (uint64)&_KernelStart;
    uint64 kernelPages = (uint64)kernelSize / 0x1000 + 1;
    
    // Lock the pages that correspond to kernel to prevent corrupting the kernel's code 
    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    //Set the page table
    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    PageTableManager::PageTableManager(PML4);

    for (uint64 t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000) {
        PageTableManager::MapMemory((void*)t, (void*)t);
    }  

    // Calculate the frame buffer size
    uint64 fbBase = (uint64)bootInfo->frameBuffer->BaseAddress;
    uint64 fbSize = (uint64)bootInfo->frameBuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
    for (uint64 t = fbBase; t < fbBase + fbSize; t += 0x1000) {
        PageTableManager::MapMemory((void*)t, (void*)t); //Map the frame buffer pages to virtual memory
    }

    asm("mov %0, %%cr3" : : "r" (PML4));
}

IDTR idtr;
void SetIDTGate(void* handler, uint8 entryOffset, uint8 type_attr, uint8 selector)
{
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

void PrepareInterrupts(BootInfo* bootInfo)
{
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64)GlobalAllocator.RequestPage(); // allocate space for IDT
    
    SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08); 
    SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITInt_Handler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm("lidt %0" : : "m" (idtr));  // Load IDT

    // PIC::Remap();
    // LAPIC::Enable(bootInfo);
}

void PrepareACPI(BootInfo* bootInfo)
{
    ACPI::XSDTHeader* xsdt = (ACPI::XSDTHeader*)(bootInfo->rsdp->XSDTAddress);
    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");
    ACPI::MADTHeader* madt = (ACPI::MADTHeader*)ACPI::FindTable(xsdt, (char*)"APIC");

    if (madt)
        APIC::ParseMADT(madt);

    kprintf("---- Enumerating PCI Devices ----\n");
    PCI::EnumeratePCI(mcfg);
    kprintf("---------------------------------\n");
}

void DrawBootImage(BootInfo* bootinfo, unsigned int xOff = 0, unsigned int yOff = 0)
{
    uint32* pixel = (uint32*)bootinfo->osulogo;
    for (unsigned int y = 0; y < 256; y++) {
        for (unsigned int x = 0; x < 256; x++) {
            GlobalRenderer.PutPix(x + xOff, y + yOff, *pixel);
            pixel++;
        }
    }
}

void InitializeKernel(BootInfo* bootInfo)
{
    GlobalRenderer = BasicRenderer(bootInfo->frameBuffer, bootInfo->psf1_font);
    
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
    kprintf("Loaded GDT\n");
     
    PrepareMemory(bootInfo);
    memset(bootInfo->frameBuffer->BaseAddress, 0, bootInfo->frameBuffer->BufferSize);
    InitializeHeap((void*)0x0000100000000000, 0x10);
    kprintf("Initialized Memory\n");

    PrepareInterrupts(bootInfo);
    kprintf("Interrupts Prepared\n"); 

    InitPS2Mouse(); 
    kprintf("Mouse Prepared\n"); 

    PrepareACPI(bootInfo);
    kprintf("ACPI Prepared\n"); 

    // outb(PIC1_DATA, 0b11111000);
    // outb(PIC2_DATA, 0b11101111);
    // kprintf("Set PIC Data\n"); 

    PIT::SetDivisor(65535);

    DrawBootImage(bootInfo, 727, 727);

    asm("sti");
}
