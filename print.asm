print:
	pusha
	mov ah, 0x0e ; TTY Mode
	
	;while(string[i] != 0) {print string[i]; i++} ;

start:	; Comparision for string end (null type)
	mov al, [bx] ; Base address for the string
	cmp al, 0	 ; Compare to see if al is 0
	je done		 ; Jump to 'done' if equals to 0

	mov ah, 0x0e ; TTY Mode
	int 0x10	 ; Since ah is set to TTY Mode, print al which is the current char

	inc bx		 ; Increment bx
	jmp start	 ; Jump back to the start

done:
	popa
	ret

newLine:
	pusha

	mov ah, 0x0e	; TTY mode
	
	mov al, 0x0a	; New line
	int 0x10
	mov al, 0x0d	; Carriage return
	int 0x10

	popa
	ret