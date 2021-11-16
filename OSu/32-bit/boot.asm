; i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "kernel.o"
; nasm "kernel_entry.asm" -f elf -o "kernel_entry.o"
; i386-elf-ld -o "full_kernel.bin" -Ttext 0x1000 "kernel_entry.o" "kernel.o" --oformat binary
; nasm "boot.asm" -f bin -o "boot.bin"
; cat "boot.bin" "full_kernel.bin" > "everything.bin"
; nasm "zeros.asm" -f bin -o "zeros.bin"
; cat "everything.bin" "zeros.bin" > "OS.bin"
; qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy, -m 128M

; learn how to %import asm
; play around with extern
; read article about moving the cursor + print()

[org 0x7c00]                    ; bootloader offset
    KERNEL_LOCATION equ 0x1000                         
    BOOT_DISK: db 0
    
    mov [BOOT_DISK], dl 
                            
    xor ax, ax                          
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp
    
    mov bx, KERNEL_LOCATION
    mov dh, 2
    mov dl, [BOOT_DISK]

    call boot_disk            

    mov ah, 0x0 
    mov al, 0x3
    int 0x10

    call switch_to_pm
    jmp $                       ;    this will actually never be executed

; include files ;
%include "../16-bit/print.asm"
%include "../16-bit/print_hex.asm"
%include "../16-bit/boot_disk.asm"

%include "GDT/GDT.asm"
%include "print-32-bit.asm"
%include "switch.asm"
%include "bootsect.asm"

[bits 32]
BEGIN_PM:                           ; after the switch go here
    call print_string_pm            ; Note that this will be written at the top left corner
    jmp KERNEL_LOCATION
    
; bootsector
times 510-($-$$) db 0
dw 0xaa55
