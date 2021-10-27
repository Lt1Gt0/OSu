[org 0x7c00]					; Set Origin to 0x7c00

get_input:						; start of the call
	pusha						; push all registers to stack for retrival later
	
	mov cx, 0					; will be used as the counter register
	mov bx, buffer				; set bx to the buffer value

loop_input:						; loop through the input for some amount of characters
	mov ah, 0x00				; set the BIOS interrupt to wait for a keypress and read the character
	cmp cx, 20					; compare the value stored in register 'CX' to 0
	je end						; if(cx == 0) jump to end

	int 0x16					; call interrupt to read keypress
	mov [bx], al				; move the value from the keypress into the adress stored in bx
	
	mov ah, 0x0e				; Enter TTY mode
	mov al, [bx]				; move the inputted character to register al
	int 0x10					; print the inputted value from the keyboard to the console
	
	inc bx						; increment bx
	inc cx						; increment cx
	jmp loop_input				; call the top of the function
		
end:							; end of the function
	call print_new_line			
	call print					; Print the value stored in register bx
	call print_new_line

	popa						; restore the previous stack
	ret							; return

buffer: times 20 db 0