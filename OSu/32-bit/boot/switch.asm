[bits 16]
switch_to_pm:
	call enable_A20
	cli 								; Clear interrupts
	lgdt [GDT_descriptor]				; Load the gdt
	mov eax, cr0						; Change the last bit of cr0 to 1
	or eax, 1
	mov cr0, eax						; now in 32-bit protected mode


	jmp CODE_SEG:start_protected_mode	; far jump

[bits 32]
start_protected_mode:
	mov ax, DATA_SEG					; Update the segment registers				
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000					; update the stack to the top of the free space
	mov esp, ebp

	call BEGIN_PM

enable_A20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret