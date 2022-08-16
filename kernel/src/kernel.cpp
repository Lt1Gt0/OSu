#include <kernelUtil.h>
#include <colors.h>
#include <print.h>

extern "C" void _start(BootInfo *bootInfo)
{ 
    InitializeKernel(bootInfo);

    GlobalRenderer.Color = Colors::TTY::OSU;
    kprintf("Kernel Initialize Sucessfully\n");
    GlobalRenderer.Color = Colors::TTY::WHITE;

    while (true) {
        asm("hlt");
    }
}
