
.section .text

.global main64
main64:
.code64

	movq	$0x2f6e2f692f612f4d, %rax
	movq	%rax, (0xb8000)

	.Lloop: 
		jmp .Lloop

