; Very Simple paging setup, subject to modification in the future ;

page_table_entry equ 0x1000		; define page table entry point at the beginning od memory

; map 2MiB in page table ;
set_up_identity_paging:
	mov edi, page_table_entry	; move the page table entry to the extended destination index register
	mov cr3, edi 				; move the extended destination index register into control register 3
								; to tell the mmu where the page table will start at
	; Make one entry for the first 3 tables ;
	mov dword [edi], 0x2003		; First entry
	add edi, 0x1000
	mov dword [edi], 0x3003		; Second Entry
	add edi, 0x1000
	mov dword [edi], 0x4003		; Third Entry
	add edi, 0x1000

	; Make 512 entries in thelast table ;
	mov ebx, 0x00000003			; Set the flag bits
	mov ecx, 512			

	; set the entry point for the linker ;
	.set_entry:
		mov dword [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop .set_entry

	; Set the PAE bit in CR4 ;
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; Set long mode bit in EFR ; 
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; Enable Paging ;
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret