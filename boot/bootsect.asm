; WORK ON BOOT SECTOR ;

boot_sect:
	pusha       

    mov [BOOT_DISK], dl 
                            
    xor ax, ax                          
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp
    
    mov bx, KERNEL_LOCATION
    mov dh, 2
    mov dl, [BOOT_DISK]

    call boot_disk            

    mov ah, 0x0 
    mov al, 0x3
    int 0x10

    call switch_to_pm

	popa
	ret