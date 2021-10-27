print_hex:
	pusha
	mov cx, 0 ; Use as counter

hex_loop:
	cmp cx, 4
	je hex_end

	mov ax, dx ; use ax as the working register
	and ax, 0x000f  ; 0x1234 -> 0x0004 by masking the first three 0's
	add al, 0x30	; add 30 to N to convert to ascii 'N'
	cmp al, 0x39	; if al > 9 add an extra '8' to represent 'A' to 'F'
	jle step2
	add al, 7
step2:
	; bx <- base address + string length - index of char ;
	mov bx, HEX_OUT + 5 ; base + length
	sub bx, cx			; index of character
	mov [bx], al		; copy ascii char on al to the position pointed by 'bx'
	ror dx, 4			; 0x1234 -> 0x4123 -> 0x4312 -> 0x4321 -> 0x1234

	inc cx
	jmp hex_loop

hex_end:
	mov bx, HEX_OUT
	call print

	popa ; restore the stack
	ret
HEX_OUT:
	db "0x0000", 0