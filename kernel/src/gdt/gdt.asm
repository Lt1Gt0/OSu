[bits 64]
LoadGDT:
    lgdt[rdi]       ; RDI register will contain initial parameter
    mov     ax, 0x10    ; Selector for kernel data segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    pop     rdi         ; return address of the GDT function
    mov     rax, 0x08   ; segment selector for kernel code segment
    push    rax
    push    rdi
    retfq
GLOBAL LoadGDT