[bits 16]
%include "assembly/DetectMemory.asm"

switch_to_64_bit:

	; Switch to protected mode ;
	switch_to_pm:
		call detect_memory
		call enable_A20
		cli 								; Clear interrupts
		lgdt [GDT_descriptor]				; Load the gdt
		mov eax, cr0						; Change the last bit of cr0 to 1
		or eax, 1
		mov cr0, eax						; now in 32-bit protected mode
	
	
		jmp CODE_SEG:start_protected_mode	; far jump
	
	; Enable to A20 line;
	enable_A20:
		in al, 0x92
		or al, 2
		out 0x92, al
		ret

	[bits 32]

	%include "assembly/CPUID.asm"
	%include "assembly/paging.asm"

	start_protected_mode:
		mov ax, DATA_SEG					; Update the segment registers				
		mov ds, ax
		mov ss, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
	
		call detect_CPUID					; Check for CPUID
		call detect_long_mode				; Check for Long Mode
		call set_up_identity_paging			; Set up paging
		call edit_GDT						; Modify the gdt for 64 bit
	
		jmp CODE_SEG:start_64_bit			; long jump to 64-bit start
	
	[bits 64]
	start_64_bit:
		call BEGIN_64_BIT