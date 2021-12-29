HEADER_DIR = kernel/src/headers
LD = kernel/kernel.ld
LINKER = ld
CPP = gcc
CPPFLAGS = -ffreestanding -mno-red-zone -m64 -masm=intel -I$(HEADER_DIR)

OBJ_FILES = assembly/sector_2/ExtendedProgram.o kernel/src/kernel.o assembly/binaries/binaries.o kernel/src/Interrupts/IDT.o kernel/src/IO/IO.o kernel/src/Input/Keyboard.o kernel/src/Memory/MemoryMap.o kernel/src/Terminal.o kernel/src/Memory/Heap.o kernel/src/Memory/Memory.o


all: OS.bin
	$(info ---------- Running OS ----------)
	qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
setup:
	$(info ---------- Setting Up Environment ----------)
	export PREFIX="/usr/local/x86_64elfgcc"
	export TARGET=x86_64-elf
	export PATH="$$PREFIX/bin:$$PATH"
OS.bin: assembly/boot_sector/bootloader.bin
	$(info ---------- Generating OS.bin ----------)
	cat $^ > $@
assembly/boot_sector/bootloader.bin: kernel/src/kernel.bin assembly/boot_sector/bootloader.asm
	$(info ---------- Generating Bootloader ----------)
	nasm assembly/boot_sector/bootloader.asm -f bin -o assembly/boot_sector/bootloader.bin
	cat kernel/src/kernel.bin >> $@
kernel/src/kernel.bin: $(OBJ_FILES) kernel/link.ld 
	$(info ---------- Linking Files ----------)
	$(LINKER) -T"kernel/link.ld" 
%.o: %.cpp
	$(CPP) -Ttext 0x8000 $(CPPFLAGS) -c $^ -o $@
%.o: %.asm 
	nasm $^ -f elf64 -o $@	
clean:
	$(info ---------- Cleaning ----------)
	find . -name "*.bin" -type f -delete
	find . -name "*.o" -type f -delete
	find . -name "*.tmp" -type f -delete
