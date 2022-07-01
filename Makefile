all:
	make -C gnu-efi/
	make bootloader -C gnu-efi/
	make kernel -C kernel/
	make buildimg -C kernel/
	
run: all
	make run -C kernel/

debug: all
	make debug -C kernel/

clean: 
	@rm kernel/lib/ -rf
	make clean -C gnu-efi/

.PHONY: all run