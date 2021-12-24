[org 0x7c00]
	mov [BOOT_DISK], dl	; prepare the BOOT_DISK to be read
	
	mov bp, 0x7c00		; move the base pointer to the origin
	mov sp, bp			; move the stack pointer to the base pointer
	
	call read_disk		; Read disks
	
	jmp PROGRAM_SPACE	; Move to the program space
	
	%include "assembly/boot_sector/print.asm"
	%include "assembly/boot_sector/disk_read.asm"
	
	; Magical boot number ;
	times 510-($-$$) db 0
	dw 0xaa55