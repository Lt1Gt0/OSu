[bits 16]
call switch_to_64_bit
jmp $
        
%include "assembly/boot_sector/switch.asm"
%include "assembly/GDT.asm"


[bits 64]
[extern _start]

%include "assembly/IDT.asm"

BEGIN_64_BIT:                           ; after the switch go here
    mov ecx, 500
    rep stosq
    call activate_SSE
    call _start
    jmp $

activate_SSE:
    mov rax, cr0
    and ax, 0b11111101
    or ax, 0b00000001
    mov cr0, rax

    mov rax, cr4
    or ax, 0b1100000000
    mov cr4, rax
    ret

times 2048-($-$$) db 0                  ; padding