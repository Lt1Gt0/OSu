#include <kernelUtil.h>
#include <memory/memory.h>
#include <paging/pageframeAllocator.h>
#include <userinput/mouse.h>
#include <gdt/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/interrupts.h>
#include <interrupts/pic.h>
#include <interrupts/apic.h>
#include <timer/pit/pit.h>
#include <print.h>

void PrepareMemory(BootInfo* bootInfo)
{
	// get the total map entries by dividing the size of the map by the descriptor size
    uint64 mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;
	
	// Declare the global allocator to an instance of a PageFrameAllocator
    GlobalAllocator = PageFrameAllocator(); 
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

	SetIDTGate((void*)DivideByZero_Handler, 0x00, IDT_TA_InterruptGate, 0x08); 
	SetIDTGate((void*)Debug_Handler, 0x01, IDT_TA_TrapGate, 0x08); 
	SetIDTGate((void*)NMI_Handler, 0x02, IDT_TA_InterruptGate, 0x08); 
	SetIDTGate((void*)Breakpoint_Handler, 0x03, IDT_TA_TrapGate, 0x08); 
	SetIDTGate((void*)Overflow_Handler, 0x04, IDT_TA_TrapGate, 0x08); 
	SetIDTGate((void*)BoundRangeExceeded_Handler, 0x05, IDT_TA_InterruptGate, 0x08); 
	SetIDTGate((void*)InvalidOpcode_Hander, 0x06, IDT_TA_InterruptGate, 0x08); 
	SetIDTGate((void*)DeviceNotAvaliable_Handler, 0x07, IDT_TA_InterruptGate, 0x08); 
    SetIDTGate((void*)DoubleFault_Handler, 0x08, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)CoprocessorSegmentOverrun_Handler, 0x09, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)InvalidTSS_Handler, 0x0A, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)SegmentNotPresent_Handler, 0x0B, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)StackSegmentFault_Handler, 0x0C, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler, 0x0D, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PageFault_Handler, 0x0E, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)x87FloatingPointException_Handler, 0x10, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)AlignmentCheck_Handler, 0x11, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)MachineCheck_Handler, 0x12, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)SIMDFloatingPointException_Handler, 0x13, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)VirtualizationException_Handler, 0x14, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)ControlProtectionException_Handler, 0x15, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)HypervisorInjectionException_Handler, 0x1C, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)VMMCommunicationException_Handler, 0x1D, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)SecurityException_Handler, 0x1E, IDT_TA_InterruptGate, 0x08);
		
    SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08); 
    SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITInt_Handler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm("lidt %0" : : "m" (idtr));  // Load IDT

    PIC::Remap();
    // LAPIC::Enable(bootInfo);
}

void PrepareACPI(BootInfo* bootInfo)
{
    ACPI::XSDTHeader* xsdt = (ACPI::XSDTHeader*)(bootInfo->rsdp->XSDTAddress);
    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");
    ACPI::MADTHeader* madt = (ACPI::MADTHeader*)ACPI::FindTable(xsdt, (char*)"APIC");

    if (madt) {
        kprintf("Parsing MADT\n");
        APIC::ParseMADT(madt);
    } else {
        kprintf("MADT not found\n");
    }

    kprintf("---- Enumerating PCI Devices ----\n");
    PCI::EnumeratePCI(mcfg);
    kprintf("---------------------------------\n");
}

void DrawBootImage(BootInfo* bootinfo, uint32 xOff = 0, uint32 yOff = 0)
{
    uint32* pixel = (uint32*)bootinfo->osulogo;
    for (uint32 y = 0; y < 256; y++) {
        for (uint32 x = 0; x < 256; x++) {
            GlobalRenderer.PutPix(x + xOff, y + yOff, *pixel);
            pixel++;
        }
    }
}

void InitializeKernel(BootInfo* bootInfo)
{
    asm("cli");

    GlobalRenderer = Renderer(bootInfo->frameBuffer, bootInfo->psf1_font);
    
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

    outb(PIC::PIC1_DATA, 0b11111000);
    outb(PIC::PIC2_DATA, 0b11101111);
    kprintf("Set PIC Data\n"); 

    PIT::SetDivisor(65535);

	// WYSI
    DrawBootImage(bootInfo, 727, 727);

    asm("sti");
}
