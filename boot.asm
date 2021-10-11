[org 0x7c00]

mov bx, BRUH
call print

call newLine

mov dx, 0x1234
call print_hex


; Freeze program ;
cli
hlt

BRUH: db "Bruh", 0

%include "print.asm"
%include "print_hex.asm"

times 	510-($-$$) db 0		; Padding
dw	0xaa55			; Magic boot value
