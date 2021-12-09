###PATH=$PATH:/usr/local/i386elfgcc/bin
###PATH=$PATH:/usr/local/x86_64elfgcc/bin (just incase the path varable isnt setup for this compiler)
##
##CPP_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp)
##HEADERS = $(wildcard kernel/*.h drivers/*.h)
##OBJS = ${CPP_SOURCES:.cpp=.o}
##
##CPP = i386-elf-gcc
##CPPFLAGS = -ffreestanding -m32 -g
##
##
##all: OS.bin
##	$(info ---------- Running OS ----------)
##	qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
##setup:
##	export PREFIX="/usr/local/i386elfgcc"
##	export TARGET=i386-elf
##	export PATH="$$PREFIX/bin:$$PATH"
##OS.bin: boot/everything.bin boot/zeros.bin
##	$(info ---------- Generating OS binary ----------)
##	cat $^ > $@
##boot/zeros.bin: boot/zeros.asm
##	$(info ---------- padding ----------)
##	nasm $^ -f bin -o $@
##boot/everything.bin: boot/boot.bin kernel/full_kernel.bin
##	$(info ---------- Generating a a complete binary file ----------)
##	cat $^ > $@
##boot/boot.bin: boot/boot.asm
##	$(info ---------- Generating boot file ----------)
##	nasm $^ -f bin -o $@
##kernel/full_kernel.bin: kernel/kernel_entry.o kernel/kernel.o
##	$(info ---------- Generating full_kernel.bin ----------)
##	i386-elf-ld -o $@ -Ttext 0x100 $^ --oformat binary
##kernel/kernel_entry.o: kernel/kernel_entry.asm
##	$(info ---------- Generating kernel.o ----------)
##	nasm $^ -f elf -o $@
##$(OUT): $(OBJFILES)
##	$(CPP) $(CPPFLAGS) -o $@ $^
##%.o: %.cpp ${HEADERS}
##	$(info ---------- Generating object files from cpp ----------)
##	$(CPP) $(CPPFLAGS) -c $< -o $@
##	@echo $^
##clean:
##	$(info ---------- Cleaning ----------)
##	rm -fr *.bin kernel/*.bin drivers/*.bin boot/*.bin
##	rm -fr *.o kernel/*.o drivers/*.o boot/*.o

#PATH=$PATH:/usr/local/i386elfgcc/bin
#PATH=$PATH:/usr/local/x86_64elfgcc/bin (just incase the path varable isnt setup for this compiler)

CPP_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJS = ${CPP_SOURCES:.cpp=.o}

CPP = i386-elf-gcc
CPPFLAGS = -ffreestanding -m32 -g

all: tmp_bootloader.bin
	$(info ---------- Running OS ----------)
	qemu-system-x86_64 -drive format=raw,file="tmp_bootloader.bin",index=0,if=floppy -m 128M
tmp_bootloader.bin: sector_2/ExtendedProgram.bin sector_1/bootloader.asm
	nasm sector_1/bootloader.asm -f bin -o tmp_bootloader.bin
	cat sector_2/ExtendedProgram.bin >> tmp_bootloader.bin
sector_2/ExtendedProgram.bin: sector_2/ExtendedProgram.asm
	nasm $^ -f bin -o $@		
#OS.bin: boot/everything.bin boot/zeros.bin
#	$(info ---------- Generating OS binary ----------)
#	cat $^ > $@
#boot/zeros.bin: boot/zeros.asm
#	$(info ---------- padding ----------)
#	nasm $^ -f bin -o $@
#boot/everything.bin: boot/boot.bin kernel/full_kernel.bin
#	$(info ---------- Generating a a complete binary file ----------)
#	cat $^ > $@
#boot/boot.o: boot/boot.asm
#	$(info ---------- Generating boot file ----------)
#	nasm $^ -f elf64 -o $@
#kernel/full_kernel.bin: kernel/kernel_entry.o kernel/kernel.o
#	$(info ---------- Generating full_kernel.bin ----------)
#	i386-elf-ld -o $@ -Ttext 0x100 $^ --oformat binary 
#kernel/kernel_entry.o: kernel/kernel_entry.asm
#	$(info ---------- Generating kernel.o ----------)
#	nasm $^ -f elf -o $@
#$(OUT): $(OBJFILES)
#	$(CPP) $(CPPFLAGS) -o $@ $^
#%.o: %.cpp ${HEADERS}
#	$(info ---------- Generating object files from cpp ----------)
#	$(CPP) $(CPPFLAGS) -c $< -o $@
#	@echo $^
clean:
	$(info ---------- Cleaning ----------)
	rm -fr *.bin kernel/*.bin drivers/*.bin boot/*.bin sector_1/*.bin sector_2/*.bin
	rm -fr *.o kernel/*.o drivers/*.o boot/*.o#