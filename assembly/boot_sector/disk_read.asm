PROGRAM_SPACE equ 0x8000
read_disk:
	mov ah, 0x02
	mov bx, PROGRAM_SPACE
	mov al, 40
	mov dl, [BOOT_DISK]
	mov ch, 0x00 
	mov dh, 0x00 
	mov cl, 0x02

	int 0x13

	jc disk_read_failed

	mov bx, MSG_DISK_READ_SUCCESS
	call print
	
	ret

BOOT_DISK: db 0

MSG_DISK_READ_ERROR: db 'Disk Failed to read', 0
MSG_DISK_READ_SUCCESS: db 'Disk Read successfully', 0

disk_read_failed:
	mov bx, MSG_DISK_READ_ERROR
	call print

	jmp $