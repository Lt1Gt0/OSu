#include <panic.h>
#include <renderer.h>
#include <print.h>
#include <colors.h>

void Panic(const char* panicMessage)
{
    /* Panic mode complete yet, will only show that something
     * happened but nothing that is to useful as to why it happened,
     * debug printing will be next for panic mode*/
    GlobalRenderer.mClearColor = Colors::TTY::RED;

    GlobalRenderer.Clear();
    GlobalRenderer.mCursorPosition = {0, 0};
    GlobalRenderer.mColor = 0;

    kprintf("Kernel Panic");
    
    GlobalRenderer.Next();

    kprintf(panicMessage);
}
