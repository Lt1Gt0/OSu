#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Print("Kernel Initialized Successfully"); 
    
    // Kerenl Main loop
    while(true){
        ProcessMousePacket();
    }

    while(true); // Prevent OS from crashing
}

