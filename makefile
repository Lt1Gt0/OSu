#PATH=$PATH:/usr/local/x86_64elfgcc/bin

CPP_SOURCES = $(wildcard src/kernel/*.cpp src/drivers/*.cpp)
#HEADERS = $(wildcard kernel/*.h drivers/*.h)
LINKERS = $(wildcard src/kernel/*.ld src/drivers/*.ld)
OBJS = ${CPP_SOURCES:.cpp=.o}

LINKER = x86_64-elf-ld
CPP = x86_64-elf-gcc
CPPFLAGS = -ffreestanding -mno-red-zone -m64

all: OS.bin
	$(info ---------- Running OS ----------)
	qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
setup:
	$(info ---------- Setting up environment ----------)
	export PREFIX="/usr/local/x86_64elfgcc"
	export TARGET=x86_64-elf
	export PATH="$$PREFIX/bin:$$PATH"
OS.bin: assembly/boot_sector/bootloader.bin
	$(info ---------- Generating OS.bin ----------)
	cat $^ > $@
assembly/boot_sector/bootloader.bin: src/kernel/kernel.bin assembly/boot_sector/bootloader.asm
	$(info ---------- Generating bootloader ----------)
	nasm assembly/boot_sector/bootloader.asm -f bin -o assembly/boot_sector/bootloader.bin
	cat src/kernel/kernel.bin >> $@
src/kernel/kernel.bin: assembly/sector_2/ExtendedProgram.o src/kernel/kernel.o src/kernel/link.ld
	$(LINKER) -T"src/kernel/link.ld" 
src/kernel/kernel.o: src/kernel/kernel.cpp 
	$(CPP) -Ttext 0x8000 $(CPPFLAGS) -c $^ -o $@
assembly/sector_2/ExtendedProgram.o: assembly/sector_2/ExtendedProgram.asm
	nasm $^ -f elf64 -o $@
clean:
	$(info ---------- Cleaning ----------)
	rm -fr *.bin src/kernel/*.bin src/drivers/*.bin assembly/boot_sector/*.bin assembly/sector_2/*.bin
	rm -fr *.o src/kernel/*.o src/drivers/*.o src/boot_sector/*.o src/sector_2/*.o
	rm -fr *.tmp assembly/boot_sector/*.tmp assembly/sector_2/*.tmp src/kernel/*.tmp 