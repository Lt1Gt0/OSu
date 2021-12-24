[extern _idt]
idtDescriptor:
	dw 4095
	dq _idt			

	%macro PUSHALL 0 	; Macro to push needed registers on stack
		push rax
		push rcx
		push rdx
		push r8
		push r9
		push r10
		push r11
	%endmacro

	%macro POPALL 0 	; Macro to pop register off the stack
		pop r11
		pop r10
		pop r9
		pop r8
		pop rdx
		pop rcx
		pop rax
	%endmacro

[extern isr1_handler]	; isr1_handler method defined as extern, later used as a C function
isr1:
	PUSHALL				
	call isr1_handler
	POPALL
	iretq
	GLOBAL isr1			; Define isr1 as global

loadIDT:
 lidt[idtDescriptor]	; load the idtDescriptor
 sti					;
 ret 					; return call
 GLOBAL loadIDT			; define loadIDT as global