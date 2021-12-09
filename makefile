#PATH=$PATH:/usr/local/x86_64elfgcc/bin

CPP_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
LINKERS = $(wildcard kernel/*.ld drivers/*.ld)
OBJS = ${CPP_SOURCES:.cpp=.o}

CPP = i386-elf-gcc
CPPFLAGS = -ffreestanding -m32 -g


all: OS.bin
	$(info ---------- Running OS ----------)
	qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
OS.bin: boot/bootloader.bin
	$(info ---------- Generating OS binary ----------)
	cat $^ > $@
boot/bootloader.bin: sector_1/bootloader.asm sector_2/ExtendedProgram.bin
	$(info ---------- Generating bootloader ----------)
	nasm sector_1/bootloader.asm -f bin -o boot/bootloader.bin
	cat sector_2/ExtendedProgram.bin >> boot/bootloader.bin
sector_2/ExtendedProgram.bin: sector_2/ExtendedProgram.asm
	nasm $^ -f bin -o $@	
boot/boot.bin: boot/bootloader.asm
	$(info ---------- Generating boot file ----------)
	nasm $^ -f bin -o $@
#kernel/full_kernel.bin: kernel/kernel_entry.o kernel/kernel.o
#	$(info ---------- Generating full_kernel.bin ----------)
#	i386-elf-ld -o $@ -Ttext 0x100 $^ --oformat binary
$(OUT): $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o $@ $^
%.o: %.cpp ${HEADERS}
	$(info ---------- Generating object files from cpp ----------)
	$(CPP) $(CPPFLAGS) -c $< -o $@
	@echo $^
clean:
	$(info ---------- Cleaning ----------)
	rm -fr *.bin kernel/*.bin drivers/*.bin boot/*.bin boot/*.bin sector_1/*.bin sector_2/*.bin
	rm -fr *.o kernel/*.o drivers/*.o boot/*.o
	rm -fr *.tmp sector_1/*.tmp sector_2/*.tmp 