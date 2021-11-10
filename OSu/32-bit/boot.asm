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
;
[org 0x7c00]                        
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

mov ah, 0x02
mov al, dh 
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK]
int 0x13                

                                    
mov ah, 0x0
mov al, 0x3
int 0x10                ; text mode


CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $
                                    
                                     
GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION

                                     
 
times 510-($-$$) db 0              
dw 0xaa55


; Include Files ;
;%include "GDT.asm"
;%include "boot_disk.asm"
;%include "../16-bit/print.asm"
;%include "../16-bit/print_hex.asm"
;%include "../16-bit/boot_disk.asm"
;$include "bootsect.asm"


; Define Variables ;
;HELLO_WORLD: db "Hello World", 0