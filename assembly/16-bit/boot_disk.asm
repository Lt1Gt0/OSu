boot_disk:
	pusha
	push dx			; Use register dx as the working register

	mov ah, 0x02	; Declare that this will read from a disk
	mov al, dh		; some number of sectors to read passed in by register dh
	mov ch, 0x00	; read from track/cylinder 0
	mov dh, 0x00	; read from head 0
	mov cl, 0x02	; Sector 2

	int 0x13		; call read disk BIOS interrupt

	jc disk_read_error	; If there is a carry bit thrown, display error

	pop dx			
	cmp dh, al		; compare the number of sectors read
	jne sectors_error ; If the sectors read does not equal the number of sectors that should
					; have been read from, then throw a disk_error

	popa			; Restore previous stack
	ret				; return

write_to_disk:
	pusha
	push dx

	mov ah, 0x03	; Declare that this will write to disk sectors
	mov al, dh		; write to a number of disks according to the parameter passed in dh
	mov ch, 0x00	; Track/cylinder 0
	mov dh, 0x00	; head 0
	mov cl, 0x02	; sector 2

	int 0x13
	
	jc disk_write_error

	pop dx
	
	mov bx, ax
	call print
	call print_new_line

	popa
	ret

disk_write_error:
	mov bx, DISK_WRITE_ERROR_MESSAGE
	call print
	call print_new_line
	mov dh, ah
	call print

disk_read_error:
	mov bx, DISK_READ_ERROR_MESSAGE	
	call print		; Print error message to console
	call print_new_line
	mov dh, ah		; ah = error code
					; dl = disk drive that had the error
	call print_hex

sectors_error:
	mov bx, SECTORS_ERROR_MESSAGE
	call print

disk_loop:
	cli
	hlt

; DECLARE VARIABLES ;
DISK_READ_ERROR_MESSAGE: db "Disk read error", 0
DISK_WRITE_ERROR_MESSAGE: db "Error Writing to the disk", 0
SECTORS_ERROR_MESSAGE: db "Incorrect number of sectors read", 0
