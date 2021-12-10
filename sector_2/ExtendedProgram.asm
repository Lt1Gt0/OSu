[bits 16]
call switch_to_64_bit
        
%include "GDT/GDT.asm"
%include "boot_sector/switch.asm"

[bits 64]
;[extern _start]
BEGIN_64_BIT:                           ; after the switch go here
    mov edi, 0xb8000
    mov rax, 0x1f201f201f201f20
    mov ecx, 500
    rep stosq
;    call _start
    jmp $

times 2048-($-$$) db 0