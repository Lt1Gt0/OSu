print_decimal:
	pusha
	mov ax, bx
	mov bx, decimal_value_buffer+4
	mov cx, 10

print_decimal_loop:	
	xor dx, dx
	div cx
	
	add dl, 0x30
	mov [bx], dl

	dec bx
	cmp ax, 0
	je print_decimal_end
	jmp print_decimal_loop

print_decimal_end:
	mov bx, decimal_value_buffer
	call print
	call print_new_line

	popa
	ret

decimal_value_buffer: times 6 db 0