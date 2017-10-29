
.data
.align 4

vgamem: .long 0xB8000
.size vgamem, 4



.section .text

#
# void println(int row, char *str, int color)
#
.global println
.type println, @function

println:

		pushl	%ebp
		movl	%esp, %ebp
		subl	$16, %esp

		// i = 0
		movl 	$0, -4(%ebp)

	.loop:
		// ax = *(str + i)
		movl	12(%ebp), %eax		# ebx = str
		addl    -4(%ebp), %eax 		# eax += i
		movzbw	(%eax), %ax 		

		cmp 	$0, %ax
		je 		.LloopE

		// bx = ax | color<<8
		movw	%ax, %bx
		movl 	16(%ebp), %ecx
		sall 	$8, %ecx
		orw     %cx, %bx


		// vgamem[(row*80 + i)] = bx
		movl 	8(%ebp), %eax
		movl	$80, %ecx
		mulw	%cx
		addl	-4(%ebp), %eax
		sall    $1, %eax
		addl	vgamem, %eax
		movw	%bx, (%eax)

		// i++
		movl    -4(%ebp), %ecx
		incl 	%ecx
		movl 	%ecx, -4(%ebp)

		jmp		.loop
	.LloopE:


		leave
		ret
.size println, .-println
