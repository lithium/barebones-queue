.extern interrupt_handler

.global isr0

.section .text
.code64



isr0:
	// divide by zero
	cli
	pushq	$0	// push a zero error code
	pushq	$0	// interrupt number
	jmp	isr_common_stub

isr1:
	// debug exception
	cli
	pushq	$0	// error code
	pushq	$1	// interrupt number
	jmp	isr_common_stub

isr2:
	// non-maskable interrupt
	cli
	pushq	$0	// error code
	pushq	$2	// interrupt number
	jmp	isr_common_stub



isr_common_stub:
		// pass in a pointer to the stackframe as first and only argument to handler
		mov	%rsp, %rdi
		call	fault_handler
		add	$16, %esp 		// erase ISR stack frame
		iretq
