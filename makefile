#PATH=$PATH:/usr/local/x86_64elfgcc/bin

CPP_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
LINKERS = $(wildcard kernel/*.ld drivers/*.ld)
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
OS.bin: boot_sector/bootloader.bin
	$(info ---------- Generating OS.bin ----------)
	cat $^ > $@
boot_sector/bootloader.bin: kernel/kernel.bin boot_sector/bootloader.asm
	$(info ---------- Generating bootloader ----------)
	nasm boot_sector/bootloader.asm -f bin -o boot_sector/bootloader.bin
	cat kernel/kernel.bin >> $@
kernel/kernel.bin: sector_2/ExtendedProgram.o kernel/kernel.o kernel/link.ld
	$(LINKER) -T"kernel/link.ld" 
kernel/kernel.o: kernel/kernel.cpp 
	$(CPP) -Ttext 0x8000 $(CPPFLAGS) -c $^ -o $@
sector_2/ExtendedProgram.o: sector_2/ExtendedProgram.asm
	nasm $^ -f elf64 -o $@
clean:
	$(info ---------- Cleaning ----------)
	rm -fr *.bin kernel/*.bin drivers/*.bin boot_sector/*.bin sector_2/*.bin
	rm -fr *.o kernel/*.o drivers/*.o boot_sector/*.o sector_2/*.o
	rm -fr *.tmp boot_sector/*.tmp sector_2/*.tmp kernel/*.tmp 