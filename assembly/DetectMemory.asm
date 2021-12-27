MemoryRegionCount: db 0
MemoryMapLocation: dw 0x500
dq 0

GLOBAL MemoryRegionCount
GLOBAL MemoryMapLocation

detect_memory:
    mov ax, 0
    mov es, ax
    mov di, [MemoryMapLocation]         ; Random location to store memory map
    mov edx, 0x534D4150                 ; SMAP
    xor ebx, ebx                        ; Clear ebx

    ; Loop through every listing in table, call bios ;
    ;        interrupt and save to destination       ;
    .repeat:
        mov eax, 0xE820
        mov ecx, 24                     ; 24 byte size
        int 0x15                        ; BIOS interrupt

        ; check if the iteration through listing is complete ;
        cmp ebx, 0
        je .finished
        add di, 24                      ; If not through all listings, add 24 to the destination index
        inc byte [MemoryRegionCount]    ; New listing in the table
        jmp .repeat             

    .finished:
        ret