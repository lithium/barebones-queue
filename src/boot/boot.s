
# multiboot1

.section .multiboot
.align 4

	.set MULTIBOOT_ALIGN, 1<<0          # align loaded modules on page boundaries
	.set MULTIBOOT_MEMINFO, 1<<1 		# provide memory map
	.set MULTIBOOT_FLAGS, MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO  
	.set MULTIBOOT_MAGIC, 0x1BADB002       
	.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) 

	.multiboot_header:
		.long MULTIBOOT_MAGIC
		.long MULTIBOOT_FLAGS
		.long MULTIBOOT_CHECKSUM




.section .text
.code32


.global _start
_start:

	// multiboot1 left us in 32bit protected mode, paging off

	.Lclear_screen:
		# clear screen
		movl	$0xb8000, %edi
		xorl	%eax, %eax
		movl 	$(80*25), %ecx
		rep stosw

	.Lok_32:
		movl	$0xb8000, %edi
		movl	$0x4f4b4f4f, (%edi)
		addl	$4, %edi
		movl	$0x4f324f33, (%edi)


.start_longmode:
	.set CPUID_BIT,  1<<21
	.set LONGMODE_BIT, 1<<29
	.set PAE_PAGING_BIT, 1<<5
	.set EFER_MSR, 0xC0000080
	.set LM_BIT, 1<<8
	.set PAGING_BIT, 1<<31

	.check_for_cpuid:
		// ecx = eax = FLAGS
		pushfw
		pop 	%eax
		movl 	%eax, %ecx

		// FLAGS |= CPUID_BIT
		xorl	$CPUID_BIT, %eax
		push 	%eax
		popfw

		// eax = FLAGS
		pushfw
		pop 	%eax

		// restore original FLAGS
		push 	%ecx
		popfw  

		// if eax == ecx: nocpuid
		xorl	%eax, %ecx
		jz 		_end


	.check_for_longmode:
		// check for extended cpuid 
		movl 	$0x80000000, %eax
		cpuid
		cmp		$0x80000001, %eax
		jb 		_end

		// check for longmode
		movl 	$0x80000001, %eax
		cpuid
		test 	$LONGMODE_BIT, %edx
		// jz 		_end


	.setup_longmode_paging:
		// identity map the first 2MB
		// 		PML4T[0]	0x1000
		// 		PDPT[0] 	0x2000
		// 		PDT[0] 		0x3000
		//		PT 			0x4000 -> 0x00000000 - 0x00200000

		// zero 4k at PML4T[0]
		movl 	$0x1000, %edi
		movl	%edi, %cr3
		xorl	%eax, %eax
		movl	$4096, %ecx
		rep stosw

		// PML4T[0] = 0x2003
		movl 	%cr3, %edi
		movl 	$0x2003, (%edi)

		// PDPT[0](0x2000) = 0x3003
		addl 	$0x1000, %edi
		movl 	$0x3003, (%edi)

		// PDPT[0](0x3000) = 0x4003
		addl 	$0x1000, %edi
		movl 	$0x4003, (%edi)

		// edi == PT (identity page)
		addl 	$0x1000, %edi

		// identity map 2MB
		movl 	$3, %ebx
		mov 	$512, %ecx
		.Lset_page_entry:
			movl 	%ebx, (%edi)
			addl 	$0x1000, %ebx
			addl	$8, %edi
			loop 	.Lset_page_entry

		.Lenable_pae_paging:
			movl	%cr4, %eax
			orl 	$PAE_PAGING_BIT, %eax
			movl	%eax, %cr4


	.switch_to_compatability_mode:

		// set EFER MSR and LM-bit
		movl	$EFER_MSR, %ecx
		rdmsr
		orl 	$LM_BIT, %eax
		wrmsr

		// enable paging
		movl	%cr0, %eax
		orl 	$PAGING_BIT, %eax
		movl	%eax, %cr0

	.load_global_descriptor_table:
		lgdt 	GDT64

		// .Lrefresh_segments:
		// movw 	$0x10, %ax
		// movw	%ax, %ds
		// movw	%ax, %es
		// movw	%ax, %fs
		// movw	%ax, %gs
		// movw	%ax, %ss


		// print 64bit success message
	.Lok_64:
		movl	$0xb8004, %edi
		movl	$0x4f344f36, (%edi)


	.Lmain64:
		# initialize stack
		// movl $stack_top, %esp
		// movl $stack_top, %ebp
		// call main
		jmp main64




_end:
	cli
	hlt
.Lhang:
	jmp .Lhang

.size _start, . - _start



.section .rodata

	GDT:
		.Null: 
		.align 32
			.long 0
			.long 0
			.byte 0
			.byte 0
			.byte 0
			.byte 0
		.Code: 
		.align 32
			.long 0
			.long 0
			.byte 0
			.byte 0b10011010
			.byte 0b00100000
			.byte 0
		.Data: 
		.align 32
			.long 0
			.long 0
			.byte 0
			.byte 0b10011010
			.byte 0b00000000
			.byte 0
	GDT_end:

	GDT32:
		.word (GDT_end - GDT - 1)
		.long (.Code - GDT)

	GDT64:
		.word (GDT_end - GDT - 1)
		.quad (.Code - GDT)





# 16kb stack
.section .bootstrap_stack, "aw", @nobits

	stack_bottom:
		.skip 16384 
	stack_top:


.section .bss
.align 4096
p4_table:
	.skip 4096
p3_table:
	.skip 4096
p2_table:
	.skip 4096
