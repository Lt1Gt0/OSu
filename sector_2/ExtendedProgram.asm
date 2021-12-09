[org 0x7e00]
call switch_to_64_bit
jmp $                              ;    this will actually never be executed

%include "GDT/GDT.asm"
%include "boot/switch.asm"

[bits 64]
;[extrn _start]
BEGIN_64_BIT:                           ; after the switch go here
    mov edi, 0xb8000
    mov rax, 0x1f202f203f204f20
    mov ecx, 500
    rep stosq
    ;jmp _start
    jmp $

times 2048-($-$$) db 0