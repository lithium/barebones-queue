
.global vga.Print

.section .text
.code64

/*
 * print(x, y, asciiz)
 * 
 *   %rdi - x
 *   %rsi - y
 *   %rdx - asciiz
 *   %rcx - colors
 *
 * RETURN
 *   None
 */
vga.Print:
	// 0xb8000 + (y*NUM_COLS + x)*2

	.set NUM_COLS, 80
	.set VGAMEM, 0xB8000

		imul	$NUM_COLS, %rsi
		add	%rdi, %rsi
		imul	$2, %rsi

		// %r8 = %rcx &0xFF
		mov	%rcx, %r8
		and	$0xFF, %r8
		shl	$8, %r8

		// %rcx = 0
		xor	%rcx, %rcx
		xor	%rax, %rax

	.Lbegin:
		// al = *(asciiz+ecx)
		movb	(%rdx,%rcx), %al

		// break if !al
		cmp	$0, %al
		jz	.Lend

		// al |= colors
		or	%r8, %rax

		// vgamem[rsi+rcx*2] = %al
		mov	%ax, VGAMEM(%rsi,%rcx,2)

		inc	%rcx
		jmp	.Lbegin

	.Lend:

		ret
