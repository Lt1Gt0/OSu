#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;
    BasicRenderer newRenderer = BasicRenderer(bootInfo->frameBuffer, bootInfo->psf1_font); // Initialize a renderer

    newRenderer.Print("Kernel Initialized Successfully");

    while(true); // Prevent OS from crashing
}

