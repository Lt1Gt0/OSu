detect_CPUID:
	pushfd					; push flags onto the stack
	pop eax

	mov ecx, eax

	xor eax, 1 << 21		; Invert ID bit stored in EFLAGS
	push eax
	popfd					; restore flags

	pushfd					; push flags onto the stack since ID might be inverted
	pop eax					; get the modified EFLAGS since id might be inverted

	push ecx
	popfd					; restore original flags

	xor eax, ecx			; if eax = 0, the CPUID cant be changed 
	jz no_CPID
	ret

detect_long_mode:
	mov eax, 0x80000001		; value to test the cpuid against
	cpuid
	test edx, 1 << 29		; test bit for long mode support
	jz no_long_mode			; if the bit for long mode support isnt flipped, there is no long mode support
	ret

no_long_mode:
	hlt 					; No long mode support

no_CPID:
	hlt 					; No CPUID Support