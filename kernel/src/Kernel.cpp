#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Print("Kernel Initialized Successfully"); 
   
    GlobalRenderer->Next();
    GlobalRenderer->Print(to_hstring((uint64_t)bootInfo->rsdp));

    GlobalRenderer->Next();
    GlobalRenderer->PutChar(*(uint8_t*)bootInfo->rsdp); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 1)); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 2)); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 3)); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 4)); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 5)); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 6)); 
    GlobalRenderer->PutChar(*((uint8_t*)bootInfo->rsdp + 7)); 

    // Kerenl Main loop
    while(true){
        ProcessMousePacket();
    }

    while(true); // Prevent OS from crashing
}

