#pragma once
#include <globals.h>
#include <BasicRenderer.h>
#include <panic.h>
#include <IO.h>
#include <userinput/keyboard.h>
#include <userinput/mouse.h>
#include <scheduling/pit/pit.h>

#define PIC1_COMMAND  0x20 // IO base address for the mater PIC
#define PIC1_DATA     0x21 // IO base address for the master PIC data
#define PIC2_COMMAND  0xA0 // IO base address for the slave PIC
#define PIC2_DATA     0xA1 // IO base address for slave PIC data
#define PIC_EOI       0x20 // End of interrupt command mode

#define ICW1_INIT     0x10 // INITIALIZATION - REQUIRED!!!
#define ICW1_ICW4     0x01 // ICW4 (not) needed
#define ICW4_8086     0x01 // 8086/88 (MCS-80/85) mode

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();
