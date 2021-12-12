OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
    assembly/sector_2/ExtendedProgram.o
	src/kernel/kernel.o
)

OUTPUT 
(
	src/kernel/kernel.bin
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