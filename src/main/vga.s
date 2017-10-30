
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
.code32

		pushl	%ebp		// enter
		movl	%esp, %ebp  // enter
		pushl   %ebx        // registers we clobber
		subl	$4, %esp 	// 1 local variable

		// i = 0
		movl 	$0, -8(%ebp)

	.loop:
		// ax = *(str + i)
		movl	12(%ebp), %eax		# ebx = str
		addl    -8(%ebp), %eax 		# eax += i
		movzbw	(%eax), %ax 		

		// break if ax == '\0'
		cmp 	$0, %ax
		je 		.LloopE

		// bx = ax | color<<8
		movw	%ax, %bx
		movl 	16(%ebp), %ecx
		sall 	$8, %ecx
		orw     %cx, %bx

		// vgamem[(row*80 + i)*2] = bx
		movl 	8(%ebp), %eax
		movl	$80, %ecx
		mulw	%cx
		addl	-8(%ebp), %eax
		sall    $1, %eax
		addl	vgamem, %eax
		movw	%bx, (%eax)

		// i++
		movl    -8(%ebp), %ecx
		incl 	%ecx
		movl 	%ecx, -8(%ebp)

		jmp		.loop
	.LloopE:


		addl 	$4, %esp 		// discard our strack frame
		popl 	%ebx			// restore registers

		leave
		ret
.size println, .-println
