boot: boot.bin
	qemu-system-x86_64 boot.bin

boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

clean:
	rm boot.bin
