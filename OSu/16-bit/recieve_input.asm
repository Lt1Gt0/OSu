get_input:						; start of the call
	pusha						; push all registers to stack for retrival later
	
	mov bp, 0x7b00				; Set the base pointer
	mov dx, bp					; Set dx to the base pointer
	mov sp, bp					; by default set the stack pointer to the base pointer since the stack is empyt
	mov cx, 0 					; Use cx as counter, mostly just for string formatting

loop_input:
	mov ah, 0x00
	cmp bl, 0x0d				; If the Scan code turns out to be 0x0d (return carriage) then jump to the end
	je print_input_loop
	cmp bl, 0x08				; compare to \b (Backspace)
	je backspace				; run backspace subroutine

	int 0x16					; Read keyboard input

	mov bx, ax					; mov ax to bx
	and bx, 0x00ff				; get the scan codes character
	push bx						; push bx onto the stack

	mov ah, 0x0e				; Enter TTY mode
	mov al, bl					; print the value stored in bl to the BIOS
	int 0x10					; Call BIOS interrupt

	jmp loop_input

print_input_loop:
	cmp dx, sp
	je end_input_loop
	cmp cx, 0 					; Check to see if this is the first iteration of the print so it wont remove the user's inputted line
	je format_next_line				
	
	mov bx, dx					; get bx ready to print
	call print
	sub dx, 2 					; Move down the stack 2 bytes since the value for the character is every 2 bytes
	jmp print_input_loop

format_next_line:
	call print_new_line
	inc cx
	jmp print_input_loop

end_input_loop:					; end of the function
	cmp bx, [bp]				; Clear the stack
	pop bx						
	jne end_input_loop

	popa						; restore the previous stack
	ret							; return

backspace:
	mov ah, 0x0e				; Enter TTY mode
	mov al, " "					; print space character to erase old char
	int 0x10					; Call BIOS interrupt
	mov al, 0x08				; move backspace char back into al
	int 0x10					; Call BIOS interrupt to move cursor back again
	xor bl, bl					; zero bl
jmp loop_input
