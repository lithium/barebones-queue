
.global vga.Print
.global vga.Println
.global vga.CursorEnable
.global vga.CursorDisable
.global vga.SetCursorPosition



.section .data

currentLine: 
.byte	0

.section .text
.code64


/*
 * SetCursorPosition(x: %rdi, y: %rsi)
 *
 */
vga.SetCursorPosition:
		// %rsi = y*VGA_WIDTH+x
		imul	$NUM_COLS, %rsi
		add	%rdi, %rsi

		// 0x3d4 <- 0x0F
		mov	$0x0F, %rax
		mov	$0x3d4, %rdx
		outb	%al, %dx

		// 0x3d5 <- position & 0xFF
		mov	%rsi, %rax
		mov	$0x3d5, %rdx
		outb	%al, %dx

		// 0x3d4 <- 0x0E
		mov	$0x0E, %rax
		mov	$0x3d4, %rdx
		outb	%al, %dx

		// 0x3d5 <- (position & 0xFF00) >> 8
		mov	%rsi, %rax
		shr	$8, %ax
		mov	$0x3d5, %rdx
		outb	%al, %dx

		ret

/*
 * CursorEnable(scanlineStart: %rdi, scanlineEnd: %rsi)
 *
 */
vga.CursorEnable:
	.set VGA_cursorStart, 0xA
	.set VGA_cursorEnd, 0xB

		// 0x3d4 <- VGA_cursorStart
		movb	$VGA_cursorStart, %al
		mov	$0x3d4, %dx
		outb	%al, %dx

		// 0x3d5 <-  0x3d5 & 0xC0 | scanlineStart
		inc	%dx
		inb	%dx, %al
		and	$0xC0, %al
		or	%rdi, %rax
		outb	%al, %dx

		// 0x3d4 <- VGA_cursorEnd
		mov	$VGA_cursorEnd, %al
		mov	$0x3d4, %dx
		outb	%al, %dx

		// 0x3e0 <-  0x3e0 & 0xE0 | scanlineEnd
		mov	$0x3e0, %dx
		inb	%dx, %al
		and	$0xE0, %al
		or	%rsi, %rax
		outb	%al, %dx

		ret


/*
 * CursorDisable()
 *
 */
vga.CursorDisable:
		mov	$VGA_cursorStart, %al
		mov	$0x3d4, %dx
		out	%al, %dx

		mov	$0x20, %al
		inc	%dx
		out	%al, %dx

		ret


/*
 * Println(asciiz: %rdi, colors: %rsi)
 *
 * RETURN
 *   None
 */
vga.Println:
		mov	%rdi, %rdx
		mov	%rsi, %rcx
		mov	$0, %rdi
		mov	currentLine, %rsi
		call	vga.Print

		incb	currentLine

		mov	$0, %rdi
		mov	currentLine, %rsi
		call	vga.SetCursorPosition
		
		ret

/*
 * Print(x: %rdi, y: %rsi, asciiz: %rdx, colors: %rcx)
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
