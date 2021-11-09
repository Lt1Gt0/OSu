[org 0x7c00]

mov bx, [integer_value]
call print_decimal

;call get_input

; freeze process;
cli
hlt

; include files ;
%include "print.asm"
%include "print_hex.asm"
%include "print_decimal.asm"
%include "recieve_input.asm"
%include "boot_disk.asm"
%include "color_palette.asm"

; define variables ;
integer_value: dw 1235, 0

; BOOT SECTOR PADDING ;
times 510-($-$$) db 0
dw 0xaa55

; Define Sectors ;
;times 256 dw "am" ; Sector 2 = 512 bytes
;times 256 dw "us" ; Sector 3 = 512 bytes