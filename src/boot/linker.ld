ENTRY(start)

SECTIONS
{
	. = 1M;

	.rodata :
	{
		/* keep multiboot header near beginning */
		KEEP(*(.multiboot))
		*(.rodata)
		. = ALIGN(4K);
	}

	.text :
	{
		*(.text)
		. = ALIGN(4K);
	}

	.data :
	{
		*(.data)
		. = ALIGN(4K);
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
		. = ALIGN(4K);
	}

}