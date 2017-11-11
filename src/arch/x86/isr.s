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

.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29

.global isr30
.global isr31
.global isr32
.global isr33
.global isr34
.global isr35
.global isr36
.global isr37
.global isr38
.global isr39

.global isr40
.global isr41
.global isr42
.global isr43
.global isr44
.global isr45
.global isr46
.global isr47
.global isr48

.global isrFF

.section .text
.code64


.macro ISR_noerrorcode vector
	cli
	pushq	$0
	pushq 	\vector
	jmp	isr_common_stub
.endm

.macro ISR vector
	cli
	pushq 	\vector
	jmp	isr_common_stub
.endm


isr_vector_table:

	isr0:	ISR_noerrorcode $0 	// divide by zero
	isr1:	ISR_noerrorcode $1 	// debug exception
	isr2:	ISR_noerrorcode $2 	// non-maskable interrupt
	isr3:	ISR_noerrorcode $3 	// breakpoint exception
	isr4:	ISR_noerrorcode $4 	// overflow exception
	isr5:	ISR_noerrorcode $5 	// bound range exception
	isr6:	ISR_noerrorcode $6 	// invalid opcode exception
	isr7:	ISR_noerrorcode $7 	// device not available exception
	isr8:	ISR $8 			// double fault exception
	isr9:	ISR_noerrorcode $9 	// coprocessor segment overrun
	isr10:	ISR $10 			// invalid tss exception
	isr11:	ISR $11 			// segment not present
	isr12:	ISR $12 			// stack fault exception
	isr13:	ISR $13 			// general protection exception
	isr14:	ISR $14 			// pagefault exception
	isr15:	ISR_noerrorcode $15 	// RESERVED
	isr16:	ISR_noerrorcode $16 	// floating point error
	isr17:	ISR_noerrorcode $17 	// alignment check exception
	isr18:	ISR_noerrorcode $18 	// machine check exception
	isr19:	ISR_noerrorcode $19 	// SIMD floating point exception
	isr20:	ISR_noerrorcode $20 	// virtualization exception

	isr21:	ISR_noerrorcode $21 	
	isr22:	ISR_noerrorcode $22 	
	isr23:	ISR_noerrorcode $23 	
	isr24:	ISR_noerrorcode $24 	
	isr25:	ISR_noerrorcode $25 	
	isr26:	ISR_noerrorcode $26 	
	isr27:	ISR_noerrorcode $27 	
	isr28:	ISR_noerrorcode $28 	
	isr29:	ISR_noerrorcode $29 	
	isr30:	ISR_noerrorcode $30 	

        isr31:  ISR_noerrorcode $31      
        isr32:  ISR_noerrorcode $32
        isr33:  ISR_noerrorcode $33      
        isr34:  ISR_noerrorcode $34      
        isr35:  ISR_noerrorcode $35      
        isr36:  ISR_noerrorcode $36      
        isr37:  ISR_noerrorcode $37      
        isr38:  ISR_noerrorcode $38      
        isr39:  ISR_noerrorcode $39      
        isr40:  ISR_noerrorcode $40      

        isr41:  ISR_noerrorcode $41      
        isr42:  ISR_noerrorcode $42      
        isr43:  ISR_noerrorcode $43      
        isr44:  ISR_noerrorcode $44      
        isr45:  ISR_noerrorcode $45      
        isr46:  ISR_noerrorcode $46      
        isr47:  ISR_noerrorcode $47      
        isr48:  ISR_noerrorcode $48      


	isrFF:	ISR_noerrorcode $0xFF 	// apic spurious interrupt?


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
