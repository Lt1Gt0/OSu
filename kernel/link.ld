OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
    sector_2/ExtendedProgram.o
	kernel/kernel.o
)

OUTPUT 
(
	kernel/kernel.bin
)

SECTIONS
{
	. = 0x8000;

	.text : ALIGN(0x1000)
	{
		*(.text)
	}

	.data : ALIGN(0x1000)
	{
		*(.data)
	}

	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}

	.bss : ALIGN(0x1000)
	{
		*(COMMON)
		*(.bss)
	}
}