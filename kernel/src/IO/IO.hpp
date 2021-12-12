#pragma once

void outb(unsigned short port, unsigned char val){
	asm volatile("out %1, %0" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port){
	unsigned char returnVal;
	asm volatile ("in %0, %1" 
		: "=a"(returnVal)
		: "Nd"(port));
	return returnVal;
}

