; Printing a character to a screen
; video memory starts at address 0xb8000
; first byte 	: character
; seconds byte	: color

[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx]
	mov ah, WHITE_ON_BLACK

	cmp al, 0
	je end_print_pm

	mov [edx], ax
	add ebx, 1
	add edx, 2
	jmp print_string_pm_loop


end_print_pm:
	popa
	ret