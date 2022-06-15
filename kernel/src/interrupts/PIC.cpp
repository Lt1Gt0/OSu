#include <interrupts/PIC.h>
#include <IO.h>

namespace PIC
{
    void Remap()
    {
        uint8_t a1, a2;
   
        /* Save masks */
        a1 = inb(PIC1_DATA);
        io_wait();    
        a2 = inb(PIC2_DATA);
        io_wait();

        /* Start initialization sequence (cascade mode) */
        outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();
        outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();

        /* Might make the offset vectors an input parameter in the
         * future if needed, for the time being they will be hard
         * coded as 0x20 for PIC1_DATA and 0x28 for PIC2_DATA */
        outb(PIC1_DATA, 0x20); // ICW2: Master PIC vector offset 
        io_wait();
        outb(PIC2_DATA, 0x28); // ICW2: Slace PIC vector offset 
        io_wait();

        outb(PIC1_DATA, 4); // ICW3: tell the Master PIC there is a slave PIC at IRQ2 (0000 01000)
        io_wait();
        outb(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
        io_wait();

        outb(PIC1_DATA, ICW4_8086);
        io_wait();
        outb(PIC2_DATA, ICW4_8086);
        io_wait();

        /* Restore saved masks */
        outb(PIC1_DATA, a1);
        io_wait();
        outb(PIC2_DATA, a2);
    }

    void EndMaster()
    {
        outb(PIC1_COMMAND, PIC_EOI);
    }

    void EndSlave()
    {
        outb(PIC2_COMMAND, PIC_EOI);
        outb(PIC1_COMMAND, PIC_EOI);
    }
}