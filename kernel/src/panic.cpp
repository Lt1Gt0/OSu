#include <panic.h>

void Panic(const char* panicMessage)
{
    /* Panic mode complete yet, will only show that something
     * happened but nothing that is to useful as to why it happened,
     * debug printing will be next for panic mode*/
    GlobalRenderer.ClearColor = 0x00ff0000;

    GlobalRenderer.Clear();
    GlobalRenderer.CursorPosition = {0, 0};
    GlobalRenderer.Color = 0;
    GlobalRenderer.PrintLine("Kernel Panic");
    
    GlobalRenderer.Next();

    GlobalRenderer.PrintLine(panicMessage);
}
