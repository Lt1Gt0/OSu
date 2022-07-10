all: bootloaderObj kernelObj

kernelObj:
	@echo ~~~~~ GENERATING KERNEL ~~~~~
	make kernel -C kernel/
	make buildimg -C kernel/

bootloaderObj:
	@echo ~~~~~ GENERATING BOOTLOADER ~~~~~
	make -C gnu-efi/
	make bootloader -C gnu-efi/

run: all
	make run -C kernel/

debug: all
	make debug -C kernel/

clean: clean_kernel clean_bootloader

clean_kernel:
	@echo ~~~~~ CLEANING KERNEL ~~~~~
	make clean -C kernel/

clean_bootloader:
	@echo ~~~~~ CLEANING BOOTLOADER ~~~~~
	make clean -C gnu-efi/

.PHONY: all run