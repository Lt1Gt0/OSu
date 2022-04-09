#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    PIT::SetDivisor(65535);

    GlobalRenderer->Print("Kernel Initialized Successfully"); 

    while(true); // Prevent OS from crashing
}