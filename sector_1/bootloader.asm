[org 0x7c00]

mov [BOOT_DISK], dl

mov bp, 0x7c00
mov sp, bp

mov bx, TEST_STRING
call print

call read_disk

mov ah, 0x0e
mov al, [PROGRAM_SPACE]
int 0x10

jmp $

%include "sector_1/print.asm"
%include "sector_1/disk_read.asm"

TEST_STRING: db 'bruh', 0

times 510-($-$$) db 0
dw 0xaa55