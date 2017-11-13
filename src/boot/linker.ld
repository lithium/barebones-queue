ENTRY(start)

SECTIONS
{
	.multiboot :
	{
		/* keep multiboot header near beginning */
		KEEP(*(.multiboot))
		. = ALIGN(4K);
	}

	.realmode 0x91000  :
	{
		build/boot_ap.o (.text)
	}


	. = 1M;

	.rodata :
	{
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