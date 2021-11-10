[org 0x7c00]
KENREL_LOCATION equ 0x1000
BOOT_DISK: db 0
mov [BOOT_DISK], dl
mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print
call print_new_line

call load_kernel
call switch_to_pm
jmp $

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print
	call print_new_line

	mov bx, KENREL_LOCATION
	mov dh, 2
	mov dl, [BOOT_DRIVE]
	call boot_disk
	ret
	
[bits 32]
BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_string_pm
	call KENREL_LOCATION
	jmp $

; DECLARE VARIABLES ;
BOOT_DRIVE: db 0
MSG_LOAD_KERNEL: db "Loading kernel into memory", 0
DISK_READ_ERROR_MESSAGE: db "Disk read error", 0
DISK_WRITE_ERROR_MESSAGE: db "Error Writing to the disk", 0
SECTORS_ERROR_MESSAGE: db "Incorrect number of sectors read", 0
