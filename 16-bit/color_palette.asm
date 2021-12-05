[org 0x7c00]

change_color_palette:
	pusha

	mov ah, 0x0B
	mov bh, dh
	mov bl, dl

	int 0x10

	popa
	ret