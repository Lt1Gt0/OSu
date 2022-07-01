#include <kernelUtil.h>

extern "C" void _start(BootInfo *bootInfo)
{
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer.PrintLine("Kernel Initialized Successfully");

    while (true) {
        asm("hlt");
    }
}