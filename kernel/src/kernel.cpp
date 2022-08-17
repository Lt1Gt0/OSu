#include <kernelUtil.h>
#include <colors.h>
#include <print.h>

extern "C" void _start(BootInfo *bootInfo)
{ 
    InitializeKernel(bootInfo);

    GlobalRenderer.mColor = Colors::TTY::OSU;
    kprintf("Kernel Initialize Sucessfully\n");
    GlobalRenderer.mColor = Colors::TTY::WHITE;

    while (true) {
        asm("hlt");
    }
}
