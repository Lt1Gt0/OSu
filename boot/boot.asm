; learn how to %import asm
; play around with extern
; read article about moving the cursor + print()


[org 0x7c00]                           ; bootloader offset
    call switch_to_64_bit
    jmp $                              ;    this will actually never be executed

%include "GDT/GDT.asm"
%include "boot/switch.asm"
;%include "bootsect.asm"

[bits 64]
BEGIN_64_BIT:                           ; after the switch go here
    mov edi, 0xb8000
    mov rax, 0x1f201f201f201f20
    mov ecx, 500
    rep stosq
    jmp $

; bootsector
times 510-($-$$) db 0
dw 0xaa55
