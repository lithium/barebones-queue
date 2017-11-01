.extern vga.Print


.section .text

.global main64
main64:
.code64

	movq	$0x2f6e2f692f612f4d, %rax
	movq	%rax, (0xb8000)

	mov	$10, %rdi
	mov	$10, %rsi
	mov	$helloz, %rdx
	mov	$0x2f, %rcx
	call	vga.Print

	.Lloop: 
		jmp .Lloop




.section .rodata

helloz:	.asciz "Hello barebones bootstrap!"