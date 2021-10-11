mov	ah, 0x0e		; TTY mode


; Simple "Hello" Print to the console ;
mov	al, 'H'
int	0x10			; Print 'H'

mov	al, 'E'
int	0x10			; Print 'E'

mov	al, 'L'			
				; Sincle 'L' is repreated and stored in register al, just interupt to 0x10 twice
int	0x10			; Print 'L'
int	0x10			; Print 'L'

mov	al, 'O'
int	0x10			; print 'O'

; Freeze program ;
cli
hlt

times 	510-($-$$) db 0		; Padding
dw	0xaa55			; Magic boot value
