OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    _KERNEL_START_ = .;
    .text : ALIGN(0x1000)
    {
        _TEXT_START_ = .;
        *(.text)
        _TEXT_END_ = .;
    }
    .data : ALIGN(0x1000)
    {
        _DATA_START_ = .;
        *(.data)
        _DATA_END_ = .;
    }
    .rodata : ALIGN(0x1000)
    {
        _RODATA_START_ = .;
        *(.rodata)
        _RODATA_END_ = .;
    }
    .bss : ALIGN(0x1000)
    {
        _BSS_START_ = .;
        *(COMMON)
        *(.bss)
        _BSS_END_ = .;
    }
    _KERNEL_END_ = .;
}
