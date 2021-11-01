[org 0x7c00]

;mov bp, 0x8000
;mov sp, bp
;
;mov bx, 0x9000
;mov dh, 2
;
;call boot_disk
;
;mov dx, [0x9000]
;call print_hex
;call print_new_line	
;mov dx, [0x9000 + 512] ; next sector
;call print_hex
;call print_new_line
;
;
;mov dh, 1
;call write_to_disk
;
;call boot_disk	
;mov dx, [0x9000]
;call print_hex
;call print_new_line

;mov dh, 0
;mov dl, 4
;call change_color_palette	
;
;call get_input

call get_input

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
integer_value: db 1, 0

; BOOT SECTOR PADDING ;
times 510-($-$$) db 0
dw 0xaa55

; Define Sectors ;
times 256 dw "am" ; Sector 2 = 512 bytes
times 256 dw "us" ; Sector 3 = 512 bytes