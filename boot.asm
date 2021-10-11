[org 0x7c00]
	mov bp, 0x8000
	mov sp, bp

	mov bx, 0x9000
	mov dh, 2 ; Read 2 sectors

	call disk_load

	mov dx, [0x9000]
	call print_hex

	call newLine

	mov dx, [0x9000 + 512]
	call print_hex



; Freeze program ;
cli
hlt

; Include files ;

%include "print.asm"
%include "print_hex.asm"
%include "boot_disk.asm"

; end of boot sector ;

times 510-($-$$) db 0		; Padding
dw 0xaa55			; Magic boot value

times 256 dw "Hello" ; Sector 2 = 512 bytes
times 256 dw "World" ; Sector 3 = 512 bytes