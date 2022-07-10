#include <panic.h>
#include <BasicRenderer.h>
#include <print.h>
#include <colors.h>

void Panic(const char* panicMessage)
{
    /* Panic mode complete yet, will only show that something
     * happened but nothing that is to useful as to why it happened,
     * debug printing will be next for panic mode*/
    GlobalRenderer.ClearColor = Colors::TTY::RED;

    GlobalRenderer.Clear();
    GlobalRenderer.CursorPosition = {0, 0};
    GlobalRenderer.Color = 0;

    kprintf("Kernel Panic");
    
    GlobalRenderer.Next();

    kprintf(panicMessage);
}
