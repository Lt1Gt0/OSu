print_decimal:
	pusha

	mov bx, decimal_value_buffer
	mov cx, 0
bruh_loop:
	mov dx, [bx]
	cmp [bl], 0
	je print_decimal_end

	cmp cx, 10
	je print_decimal_end

	call print_hex
	call print_new_line

	inc cx
	inc bx
	jmp bruh_loop

	mov cx, 0
get_total_digits:
	cmp cx, 3
	je start_decimal_loop

	inc cx
	jmp get_total_digits	

start_decimal_loop:
	mov ax, bx

	mov byte [total_digits_in_val], cl
	mov bx, 0x0000
	mov bl, [total_digits_in_val]
	add bx, decimal_value_buffer
	
	mov cx, 10

print_decimal_loop:	
	xor dx, dx
	div cx
	
	add dl, 0x30
	mov [bx], dl

	call print_hex
	call print_new_line

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
total_digits_in_val: db 0