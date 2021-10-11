disk_load: ; load 'dh' sectors from drive 'dl' to ES:BX
	pusha

	;Reading from disck requires certain values in each register
	;now it will overwrite the input params for 'dx'
	;save it to the stack for later use

	push dx

	mov ah, 0x02 ; Read Disk Sectors
	mov al, dh ; number of sectors to read from (0x01 .. 0x80)
	mov cl, 0x02 ; sector (0x01 .. 0x11)

	mov ch, 0x00 ; cylinder (0x0 .. 03ff, upper 2 bits in 'cl')
	; dl is the drive number. the caller is set to it as a param and gets it from bios
	; (0=floppy, 1=floppy2, 0x80=hdd, 0x81=hdd2)
	mov dh, 0x00 ; head number (0x0 .. 0xF)

	;[es:bx]: The pointer to buffer where the data will be stored
	; caller sets it up for us, it is the standard location for int 13h
	int 0x13	; BIOS inturrupt
	jc disk_error ; if error (stored in the carry bit)

	pop dx
	cmp al, dh	; BIOS will also set 'al' to the # of sectors read. now compare it
	jne sectors_error
	popa
	ret
disk_error:
	mov bx, DISK_ERROR
	call print
	call newLine
	mov dh, ah ; ah = error code, dl = disk drive the dropped the error
	call print_hex

sectors_error:
	mov bx, SECTORS_ERROR
	call print_hex

disk_loop:
	cli
	hlt

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0