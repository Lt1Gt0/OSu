[org 0x7c00]					; Set Origin to 0x7c00

get_input:						; start of the call
	pusha						; push all registers to stack for retrival later
	
	mov bp, 0x7b00
	mov cx, bp
	mov sp, bp

	;mov cx, 0					; will be used as the counter register
	;mov bx, buffer				; set bx to the buffer value

loop_input:
	mov ah, 0x00
	cmp bl, 0x0d				; If the Scan code turns out to be 0x0d (return carriage) then jump to the end
	je print_input_loop
	int 0x16

	mov bx, ax					; mov ax to bx
	and bx, 0x00ff				; get the scan codes character
	push bx

	mov ah, 0x0e				; Enter TTY mode
	mov al, bl					; print the value stored in bl to the BIOS
	int 0x10					; Call BIOS interrupt

	jmp loop_input

print_input_loop:
	mov ah, 0x0e
	cmp cx, sp
	je end_input_loop

	mov al, 0x0d
	int 0x10
	mov al, 0x0a
	int 0x10

	;mov al, ch 
	;int 0x10
	
	sub cx, 2
	mov dx, cx
	call print_hex

	mov al, 0x0d
	int 0x10
	mov al, 0x0a
	int 0x10

	mov dx, sp
	call print_hex
	int 0x10

	jmp print_input_loop

;loop_input:						; loop through the input for some amount of characters
;	mov ah, 0x00				; set the BIOS interrupt to wait for a keypress and read the character
;	cmp cx, 20					; compare the value stored in register 'CX' to 0
;	je end						; if(cx == 0) jump to end
;
;	int 0x16					; call interrupt to read keypress
;	mov [bx], al				; move the value from the keypress into the adress stored in bx
;	
;	mov ah, 0x0e				; Enter TTY mode
;	mov al, [bx]				; move the inputted character to register al
;	int 0x10					; print the inputted value from the keyboard to the console
;	
;	inc bx						; increment bx
;	inc cx						; increment cx
;	jmp loop_input				; call the top of the function
		
end_input_loop:					; end of the function
	call print_new_line			
	call print					; Print the value stored in register bx
	call print_new_line

	popa						; restore the previous stack
	ret							; return

buffer: times 20 db 0