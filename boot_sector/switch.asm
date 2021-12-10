switch_to_64_bit:
	switch_to_pm:
		call enable_A20
		cli 								; Clear interrupts
		lgdt [GDT_descriptor]				; Load the gdt
		mov eax, cr0						; Change the last bit of cr0 to 1
		or eax, 1
		mov cr0, eax						; now in 32-bit protected mode
	
	
		jmp CODE_SEG:start_protected_mode	; far jump
	
	enable_A20:
		in al, 0x92
		or al, 2
		out 0x92, al
		ret

	[bits 32]

	%include "kernel/CPUID.asm"
	%include "kernel/paging.asm"

	start_protected_mode:
		mov ax, DATA_SEG					; Update the segment registers				
		mov ds, ax
		mov ss, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
	
		call detect_CPUID
		call detect_long_mode
		call set_up_identity_paging
		call edit_GDT
	
		jmp CODE_SEG:start_64_bit
	
	[bits 64]
	start_64_bit:
		call BEGIN_64_BIT