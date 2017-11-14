

.section .text
.code16


ap_start:
		cli

	// spinlock to acquire mutex at 0x9010
	spin_lock:
		mov	$1, %cx
	.Lspin_lock_retry:
		xor	%ax, %ax
	lock	cmpxchg	%cx, (0x9010)
		jnz	.Lspin_lock_retry

		// increment processor count at 0x9000
		mov	(0x9000), %ax
		inc	%ax
		mov	%ax, (0x9000)

		// release mutex
	spin_unlock:
		movw	$0, (0x9010)

		// save processor number in DX
		mov	%ax, %dx


real_to_protectedmode:
.code32

		lgdt	GDT32.pointer


		// protected mode enable
		mov	%cr0, %eax
		or	$1, %ax
		mov	%eax, %cr0


	.Lloop:
		cli
		hlt
		jmp .Lloop


		ljmp	$8, $protectedmode_start

protectedmode_start:
.code32
		mov	$16, %ax
		mov	%ax, %ss
		mov	%ax, %ds
		mov	%ax, %es
		mov	%ax, %fs
		mov	%ax, %gs


	.Lloop2:
		cli
		hlt
		jmp .Lloop2




.section .rodata

GDT32:
	.null32:
		.quad 0			
	.code32:
		.word 0xFFFF		// limit[0:15]
		.word 0			// base[0:15]
		.byte 0			// base[16:23]
		.byte 0b10011010 	// accessbyte = present | ring=0 | executable | code | readable
		.byte 0b11001111	// flags, limit[16:19]
		.byte 0			// base[24:31]
	.data32:
		.word 0xFFFF		// limit[0:15]
		.word 0			// base[0:15]
		.byte 0			// base[16:23]
		.byte 0b10010010 	// accessbyte = present | ring=0 | data | writable
		.byte 0b11001111	// flags, limit[16:19]
		.byte 0			// base[24:31]

GDT32.pointer:
		.word (. - GDT32 - 1)
		.quad GDT32
