[bits 16]
call switch_to_64_bit
jmp $
        
%include "boot_sector/switch.asm"
%include "GDT/GDT.asm"

[bits 64]
[extern _start]
BEGIN_64_BIT:                           ; after the switch go here
    mov edi, 0xb8000
    mov rax, 0xDf20Df20Df20Df20
    mov ecx, 500
    rep stosq
    call _start
    jmp $

times 2048-($-$$) db 0