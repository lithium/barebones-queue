
.section .text

.global long_mode_start
long_mode_start:
.code64

	movq	$0x2f592f412f4b2f4f, %rax
	movq	%rax, (0xb8000)

	.Lloop: 
		jmp .Lloop

	# pushl	%ebp
	# movl	%esp, %ebp


	# movl    $8, %ebx

	# // println(hello, ebx, 7)
	# movl	hello, %eax
	# pushl 	$7
	# pushl	%eax
	# pushl	%ebx
	# call	println
	# addl	$12, %esp 

	# incl    %ebx

	# // println(hello, ebx, 7)
	# movl	hello, %eax
	# pushl 	$7
	# pushl	%eax
	# pushl	%ebx
	# call	println
	# addl	$12, %esp 

	# leave
	# ret

