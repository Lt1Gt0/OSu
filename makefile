#PATH=$PATH:/usr/local/x86_64elfgcc/bin

CPP_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
LINKERS = $(wildcard kernel/*.ld drivers/*.ld)
OBJS = ${CPP_SOURCES:.cpp=.o}

LINKER = x86_64-elf-ld
CPP = x86_64-elf-gcc
CPPFLAGS = -ffreestanding -mno-red-zone -m64

#nasm boot_sector/bootloader.asm -f bin -o boot_sector/bootloader.bin
#nasm sector_2/ExtendedProgram.asm -f elf64 -o sector_2/ExtendedProgram.o
#x86_64-elf-gcc -ffreestanding -mno-red-zone -m64 -c kernel/kernel.cpp -o kernel/kernel.o
#x86_64-elf-ld -o kernel/kernel.tmp -Ttext 0x7e00 sector_2/ExtendedProgram.o kernel/kernel.o 
#objcopy -O binary kernel/kernel.tmp kernel/kernel.bin
#cat kernel/kernel.bin >> boot_sector/bootloader.bin
#cat boot_sector/bootloader.bin > OS.bin
#qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M


all: OS.bin
	$(info ---------- Running OS ----------)
	qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
OS.bin: boot_sector/bootloader.bin
	$(info ---------- Generating OS binary ----------)
	cat $^ > $@
boot_sector/bootloader.bin: boot_sector/bootloader.asm sector_2/ExtendedProgram.bin
	$(info ---------- Generating bootloader ----------)
	nasm boot_sector/bootloader.asm -f bin -o boot_sector/bootloader.bin
	cat sector_2/ExtendedProgram.bin >> boot_sector/bootloader.bin
sector_2/ExtendedProgram.bin: sector_2/ExtendedProgram.asm
	nasm $^ -f bin -o $@	
boot_sector/boot.bin: boot_sector/bootloader.asm
	$(info ---------- Generating boot file ----------)
	nasm $^ -f bin -o $@
kernel/kernel.bin: kernel/kernel.tmp 
	objcopy -O binary $^ $@
kernel/kernel.tmp: ExtendedProgram.o kernel/kernel.o 
	$(LINKER) -o $@ -Ttext 0x7e00 $^
$(OUT): $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o $@ $^
%.o: %.cpp ${HEADERS}
	$(info ---------- Generating object files from cpp ----------)
	$(CPP) $(CPPFLAGS) -c $< -o $@
	@echo $^
clean:
	$(info ---------- Cleaning ----------)
	rm -fr *.bin kernel/*.bin drivers/*.bin boot_sector/*.bin sector_2/*.bin
	rm -fr *.o kernel/*.o drivers/*.o boot_sector/*.o sector_2/*.o
	rm -fr *.tmp boot_sector/*.tmp sector_2/*.tmp kernel/*.tmp 