#pragma once
#ifndef _PIC_H
#define _PIC_H

namespace PIC
{
    #define PIC1_COMMAND  0x20 // IO base address for the mater PIC
    #define PIC1_DATA     0x21 // IO base address for the master PIC data
    #define PIC2_COMMAND  0xA0 // IO base address for the slave PIC
    #define PIC2_DATA     0xA1 // IO base address for slave PIC data
    #define PIC_EOI       0x20 // End of interrupt command mode
    
    #define ICW1_INIT     0x10 // INITIALIZATION - REQUIRED!!!
    #define ICW1_ICW4     0x01 // ICW4 (not) needed
    #define ICW4_8086     0x01 // 8086/88 (MCS-80/85) mode

    void Remap();
    void EndMaster();
    void EndSlave();

    void Disable();
}

#endif // _PIC_H