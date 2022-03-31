#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Print("Kernel Initialized Successfully"); 
   
    GlobalRenderer->Next();
    GlobalRenderer->Print(to_hstring((uint64_t)bootInfo->rsdp));

    // Kerenl Main loop
    while(true){
        ProcessMousePacket();
    }

    while(true); // Prevent OS from crashing
}

