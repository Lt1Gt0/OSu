[org 0x7c00]
jmp switch_to_pm


switch_to_pm:
cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

[bits 32]
start_protected_mode:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp
	jmp begin_pm

begin_pm:
	mov al, 'A'
	mov ah, 0x0f
	mov [0xb8000], ax
	jmp $

%include "GDT.asm"

times 510-($-$$) db 0
dw 0xaa55