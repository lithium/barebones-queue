.extern interrupt_handler

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20

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

isr3:
	// breakpoint exception 
	cli
	pushq	$0	// error code
	pushq	$3	// interrupt number
	jmp	isr_common_stub

isr4:
	// overflow exception 
	cli
	pushq	$0	// error code
	pushq	$4	// interrupt number
	jmp	isr_common_stub

isr5:
	// bound range exception 
	cli
	pushq	$0	// error code
	pushq	$5	// interrupt number
	jmp	isr_common_stub

isr6:
	// invalid opcode exception 
	cli
	pushq	$0	// error code
	pushq	$6	// interrupt number
	jmp	isr_common_stub

isr7:
	// device not available exception 
	cli
	pushq	$0	// error code
	pushq	$7	// interrupt number
	jmp	isr_common_stub

isr8:
	// double fault exception 
	cli
	// errocode pushed by cpu
	pushq	$8	// interrupt number
	jmp	isr_common_stub

isr9:
	// coprocessor segment overrun
	cli
	pushq	$0	// error code
	pushq	$9	// interrupt number
	jmp	isr_common_stub

isr10:
	// invalid tss exception 
	cli
	// errocode pushed by cpu
	pushq	$10	// interrupt number
	jmp	isr_common_stub

isr11:
	// segment not present 
	cli
	// errocode pushed by cpu
	pushq	$11	// interrupt number
	jmp	isr_common_stub

isr12:
	// stack fault exception
	cli
	// errocode pushed by cpu
	pushq	$12	// interrupt number
	jmp	isr_common_stub

isr13:
	// general protection exception
	cli
	// errocode pushed by cpu
	pushq	$13	// interrupt number
	jmp	isr_common_stub

isr14:
	// pagefault exception
	cli
	// errocode pushed by cpu
	pushq	$14	// interrupt number
	jmp	isr_common_stub

isr15:
	// RESERVED
	cli
	pushq	$0
	pushq	$15	// interrupt number
	jmp	isr_common_stub

isr16:
	// floating point error
	cli
	pushq	$0	// error code
	pushq	$16	// interrupt number
	jmp	isr_common_stub

isr17:
	// alignment check exception
	cli
	pushq	$0	// error code
	pushq	$17	// interrupt number
	jmp	isr_common_stub

isr18:
	// machine check exception
	cli
	pushq	$0	// error code
	pushq	$18	// interrupt number
	jmp	isr_common_stub

isr19:
	// SIMD floating point exception
	cli
	pushq	$0	// error code
	pushq	$19	// interrupt number
	jmp	isr_common_stub

isr20:
	// virualization exception
	cli
	pushq	$0	// error code
	pushq	$20	// interrupt number
	jmp	isr_common_stub

isr_common_stub:
		// push additional registers onto the isr frame
		mov	%cr2, %rax
		pushq	%rax

		// pass in a pointer to the stackframe as first and only argument to handler
		mov	%rsp, %rdi

		call	fault_handler

		popq	%rax
		
		add	$16, %esp 		// erase ISR stack frame
		iretq
