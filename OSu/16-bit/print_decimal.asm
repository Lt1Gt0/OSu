print_decimal:
	pusha
	mov ah, 0x0e
	mov cx, 0

print_decimal_loop:
	cmp cx, 5
	je end_print
	call print
	call print_new_line

	inc bx	
	inc cx
	jmp print_decimal_loop

print_decimal_end:
	popa
	ret