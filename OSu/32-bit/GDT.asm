gdt_start:
	dd 0x0 ; 4 bytes
	dd 0x0 ; 4 bytes

code_descriptor:
	dw 0xffff	; Segment length, bits 0-15
	dw 0x0		; Segment base, bits 0-15
	db 0x0		; Segment base, bits 16-23
	db 10011010b ; Type flags
	db 11001111b ; Other flags
	db 0

data_descriptor:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; size
 	dd gdt_start			   ; start

 CODE_SEG equ code_descriptor - gdt_start
 DATA_SEG equ data_descriptor - gdt_start