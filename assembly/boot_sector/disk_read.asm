PROGRAM_SPACE equ 0x8000				; Define where the program space is located
read_disk:
	mov ah, 0x02						; BIOS interrupt to read from disk sectors
	mov bx, PROGRAM_SPACE				; pointer to buffer
	mov al, 60							; read sectors 
	mov dl, [BOOT_DISK]					; Drive number
	mov ch, 0x00 						; Read from track 0
	mov dh, 0x00 						; Read from head 0
	mov cl, 0x02						; Read from sector 2

	int 0x13							; Read disks

	jc disk_read_failed					; if CF - 1, the disk failed to read

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