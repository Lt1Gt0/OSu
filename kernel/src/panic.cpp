#include <panic.h>
#include <renderer.h>
#include <print.h>
#include <colors.h>

void Panic(InterruptFrame* frame, const char* panicMessage)
{
    /* Panic mode complete yet, will only show that something
     * happened but nothing that is to useful as to why it happened,
     * debug printing will be next for panic mode*/

    GlobalRenderer.mClearColor = Colors::TTY::WYSI;

    GlobalRenderer.Clear();
    GlobalRenderer.mCursorPosition = {0, 0};
    GlobalRenderer.mColor = 0;

    kprintf("Kernel Panic\n");
    kprintf(panicMessage);

    kprintf("\nRIP: 0X%X0\n", frame->rip);
    kprintf("CS: 0X%X0\n", frame->cs);
    kprintf("RFLAGS: 0X%X0\n", frame->rflags);
    kprintf("RSP: 0X%X0\n", frame->rsp);
    kprintf("SS: 0X%X0\n", frame->ss);
}
