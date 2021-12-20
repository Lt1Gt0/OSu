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
    call _start
    jmp $

times 2048-($-$$) db 0