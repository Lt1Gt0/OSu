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
%include "16-bit/print.asm"
%include "16-bit/print_hex.asm"
%include "16-bit/boot_disk.asm"

%include "GDT/GDT.asm"
%include "kernel/print-32-bit.asm"
%include "boot/switch.asm"
;%include "bootsect.asm"

[bits 32]
BEGIN_PM:                           ; after the switch go here
    jmp KERNEL_LOCATION

; bootsector
times 510-($-$$) db 0
dw 0xaa55
