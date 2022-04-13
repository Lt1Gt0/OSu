#include <kernelUtil.h>

KernelInfo kernelInfo;
void PrepareMemory(BootInfo *bootInfo)
{
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize; // get the total map entries by dividing the size of the map by the descriptor size

    GlobalAllocator = PageFrameAllocator(); // Declare the global allocator to an instance of a PageFrameAllocator
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    // Get the kernel size 
    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 0x1000 + 1;
    
    // Lock the pages that correspond to kernel to prevent corrupting the kernel's code 
    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    //Set the page table
    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    g_PageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000) {
        g_PageTableManager.MapMemory((void*)t, (void*)t);
    }  

    // Calculate the frame buffer size
    uint64_t fbBase = (uint64_t)bootInfo->frameBuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->frameBuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000) {
        g_PageTableManager.MapMemory((void*)t, (void*)t); //Map the frame buffer pages to virtual memory
    }

    asm("mov %0, %%cr3" : : "r" (PML4));

    kernelInfo.pageTableManager = &g_PageTableManager;
}

IDTR idtr;
void SetIDTGate(void *handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector)
{
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

void PrepareInterrupts()
{
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)GlobalAllocator.RequestPage(); // allocate space for IDT
    
    SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08); 
    SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITInt_Handler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm("lidt %0" : : "m" (idtr));  // Load IDT

    RemapPIC();
}

void PrepareACPI(BootInfo *bootInfo)
{
    ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(bootInfo->rsdp->XSDTAddress);
    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

    GlobalRenderer->PrintLine("---- Enumerating PCI Devices ----");
    PCI::EnumeratePCI(mcfg);
    GlobalRenderer->PrintLine("---------------------------------");
}

void DrawBootImage(BootInfo* bootinfo, unsigned int xOff = 0, unsigned int yOff = 0)
{
    uint32_t* pixel = (uint32_t*)bootinfo->osulogo;
    for (unsigned int y = 0; y < 256; y++) {
        for (unsigned int x = 0; x < 256; x++) {
            GlobalRenderer->PutPix(x + xOff, y + yOff, *pixel);
            pixel++;
        }
    }
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo InitializeKernel(BootInfo *bootInfo)
{
    r = BasicRenderer(bootInfo->frameBuffer, bootInfo->psf1_font);
    GlobalRenderer = &r;
    
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
    GlobalRenderer->PrintLine("Loaded GDT");
     
    PrepareMemory(bootInfo);
    memset(bootInfo->frameBuffer->BaseAddress, 0, bootInfo->frameBuffer->BufferSize);
    InitializeHeap((void*)0x0000100000000000, 0x10);
    GlobalRenderer->PrintLine("Initialized Memory");

    PrepareInterrupts();
    GlobalRenderer->PrintLine("Interrupts Prepared"); 

    InitPS2Mouse(); 
    GlobalRenderer->PrintLine("Mouse Prepared"); 

    PrepareACPI(bootInfo);
    GlobalRenderer->PrintLine("ACPI Prepared"); 

    outb(PIC1_DATA, 0b11111000);
    outb(PIC2_DATA, 0b11101111);
    GlobalRenderer->PrintLine("Set PIC Data"); 

    PIT::SetDivisor(65535);

    DrawBootImage(bootInfo, 727, 727);

    asm("sti");

    return kernelInfo;
}
