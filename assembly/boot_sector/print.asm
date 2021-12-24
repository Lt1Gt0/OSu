; Simple 16-bit print function from BIOS interrupts ;
print:
	pusha			; push registers to the stack to return the previous values at the end of the function
	mov ah, 0x0e	; TTY Mode

print_loop:
	
	mov al, [bx]	; load the value at address bx into al
	cmp al, 0		; if al == 0
	je end_print	; jump to end_print

	int 0x10		; print the value at adress bx

	inc bx			; increment on bx
	jmp print_loop	; loop to next character in string

end_print:
	popa			; Restore the stack
	ret				; Return

print_new_line:
	pusha
	mov ah, 0x0e	; declare TTY mode
	mov al, 0xa		; move return carriage byte to al
	int 0x10
	mov al, 0xd		; move new line byte to al
	int 0x10
	jmp end_print	; end function