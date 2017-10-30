
.section .rodata
.Lhello:	
	.string "hello println!"


.section .data

hello:
	.long .Lhello
	.size hello, 4


.section .text

.global main
.type main, @function
main:
.code32

	pushl	%ebp
	movl	%esp, %ebp


	movl    $8, %ebx

	// println(hello, ebx, 7)
	movl	hello, %eax
	pushl 	$7
	pushl	%eax
	pushl	%ebx
	call	println
	addl	$12, %esp 

	incl    %ebx

	// println(hello, ebx, 7)
	movl	hello, %eax
	pushl 	$7
	pushl	%eax
	pushl	%ebx
	call	println
	addl	$12, %esp 

	leave
	ret

.size main, . - main
