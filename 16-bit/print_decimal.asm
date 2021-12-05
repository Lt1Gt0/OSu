data_storage_bp: equ 0x7b00

print_decimal:
	pusha
	mov ax, bx				; integer value passed through

	mov bp, data_storage_bp	; set up the stack pointer and base pointer
	mov sp, bp

; Get the digit count ;
mov cx, 10					; set the divisor to 10
get_digits:
	xor dx, dx 				; effectively clear register dx
	div cx					; divide ax by cx (10)

	add dl, 0x30			; Get the correct ascii value for the number that was given in dl (remainder)
	push dx					; push dx to the stack

	inc bx					; increment bx by 1 and move to the next digit
	cmp ax, 0
	je after_get_digits		; Jump to end of the function call
	jmp get_digits			; repeat

after_get_digits:
	mov dx, sp
print_decimal_loop:
	mov bx, dx				; get the current hex value on the stack and print
	call print

	cmp dx, data_storage_bp ; check if the stack pointer is the same as the base pointer
	je print_decimal_end	

	add dx, 2 				; increase stack pointer by 2 to get the nex hex val	
	jmp print_decimal_loop	
print_decimal_end:
	cmp bx, [bp]			; Clear the stack
	pop bx						
	jne print_decimal_end

	popa
	ret