[org 0x7c00]

print_decimal:
	pusha
	mov ah, 0x0e
	mov cx, 0 		; Use as counter

decimal_loop:
	mov al, [bx]
	cmp al, 0
	je decimal_end

	int 0x10

	inc bx
	inc cx
	jmp decimal_loop

decimal_end:

	popa
	ret