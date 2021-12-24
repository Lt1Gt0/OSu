; The GDT should start with a null 8 bytes ;
null_descriptor:
	dd 0x0
	dd 0x0

; GDT for the code segment ;
;    base = 0x00000000     ;
;     length = 0xffff      ;
code_descriptor:									; Base 0 (32 bit)
	dw 0xffff										; limit: 0xffff
	dw 0x0 											; 16 bits + 	pres, priv, type = 1001
		 											; 8 bits = 24	type flags 		 = 1010
													;				other flags 	 = 1100
	db 0x0
	db 10011010b									; p,p,t Type flags
	db 11001111b									; Other + limit (last 4 bytes)
	db 0x0 											; last 8 bits of the base

;                      GDT for data segment                           ;
; set up the same as the code segment with minor changes to the flags ;
data_descriptor:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b	
	db 11001111b
	db 0x0

GDT_end:

; GDT descriptor ;
GDT_descriptor:
	dw GDT_end - null_descriptor - 1 				; size (16 bit), always one less of its real size
	dq null_descriptor								; address (32 bit)

CODE_SEG equ code_descriptor - null_descriptor		; Declare the CODE_SEG
DATA_SEG equ data_descriptor - null_descriptor		; Declare the DATA_SEG

; Modify the gdt for 64 bit mode ;
[bits 32]											
edit_GDT:
	mov [code_descriptor + 6], byte 10101111b		; Modified for 64-bit 
	mov [data_descriptor + 6], byte 10101111b		; Modified for 64-bit
	ret
[bits 16]