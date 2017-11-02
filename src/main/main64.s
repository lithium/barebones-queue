.extern vga.Print


.section .text

.global main64
main64:
.code64
		movq	$0x2f6e2f692f612f4d, %rax
		movq	%rax, (0xb8000)


		// enable block cursor
		mov	$0x0, %rdi
		mov	$0xF, %rsi
		call	vga.CursorEnable


		// print a string to arbitrary place
		mov	$40, %rdi
		mov	$12, %rsi
		mov	$helloz, %rdx
		mov	$0x2f, %rcx
		call	vga.Print


		// println 10x
		mov	$10, %r10 
	.Llines:

		mov	$helloz, %rdi
		mov	$0x7, %rsi
		call	vga.Println

		dec	%r10
		jnz	.Llines


	.Lloop: 
		jmp .Lloop




.section .rodata

helloz:	.asciz "Hello barebones bootstrap!"