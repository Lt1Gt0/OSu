cpps = kernel.cpp
objects = kernel.o kernel_entry.o kernel.o
binarys = boot.bin full_kernel.bin zeros.bin everything.bin OS.bin
assemblys = boot.asm kernel_entry.asm
CC = gcc

all: OS.bin
	qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
setup:
	export PREFIX="/usr/local/i386elfgcc"
	export TARGET=i386-elf
	export PATH="$$PREFIX/bin:$$PATH"
OS.bin: everything.bin zeros.bin
	cat "everything.bin" "zeros.bin" > "OS.bin"
zeros.bin: zeros.asm
	nasm "zeros.asm" -f bin -o "zeros.bin"
everything.bin: boot.bin full_kernel.bin
	cat "boot.bin" "full_kernel.bin" > "everything.bin"
boot.bin: boot.asm
	nasm "boot.asm" -f bin -o "boot.bin"
full_kernel.bin: kernel_entry.o kernel.o
	i386-elf-ld -o "full_kernel.bin" -Ttext 0x100 "kernel_entry.o" "kernel.o" --oformat binary
kernel_entry.o: kernel_entry.asm
	nasm "kernel_entry.asm" -f elf -o "kernel_entry.o"
kernel.o: kernel.cpp
	i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "kernel.o"
		

clean:
	for file in $(binarys); do rm $${file}; done
