print_hex:
	pusha
	mov cx, 0 ; Counter for hex_loop

hex_loop:
	cmp cx, 4 ; Is the value in cx = 4?
	je  end	  ; Yes, jump to end

	; Convert last char of 'dx' to ascii ;
	mov ax, dx		; 'ax will be the working register
	and ax, 0x000f  ; 0x1234 -> 0x0004 by masking the first three 0's
	add al, 0x30	; add 30 to N to convert it to ascii "N"
	cmp al, 0x39	; if al > 9, add extra '8' to represent 'A' to 'F'
	jle step2		; if al <= 9 jump to step2
	add al, 7		; add 7 to al
	
; get correct position of string to put the ascii character ;
step2:
	; bx <- base adress + string length - index of char ;
	mov bx, HEX_OUT + 5 ; base + length 
	sub bx, cx		   ; Index of variable
	mov [bx], al 	   ; copy the ASCII char on 'al' to the position pointed to by 'bx'
	ror dx, 4 		   ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

	inc cx			   ; Increment index and loop
	jmp hex_loop

end:
	mov bx, HEX_OUT
	call print

	popa
	ret


HEX_OUT:
	db "0x0000", 0	; Reserver memory for new string