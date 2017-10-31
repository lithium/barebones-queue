ENTRY(start)

SECTIONS
{
	. = 1M;

	.rodata :
	{
		/* keep multiboot header near beginning */
		KEEP(*(.multiboot))
		*(.rodata)
	}

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

}