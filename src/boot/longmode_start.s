.global longmode_start

.section .text

longmode_start:
.code64
	
		// clear the segment registers
		mov	$0, %ax
		mov	%ax, %ss
		mov	%ax, %ds
		mov	%ax, %es
		mov	%ax, %fs
		mov	%ax, %gs

		// OKAY means we are in full long mode
		movq	$0x2f592f412f4b2f4f, %rax
		movq	%rax, (0xb8000)

		call	main64
		ret


