
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
	movl 	$stack_top, %esp 

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
		jz 		_end

		// print 64bit success message
	.Lok_64:
		movl	$0xb8004, %edi
		movl	$0x4f344f36, (%edi)

	.setup_longmode_paging:
		// p4 -> p3
		movl    $p4_table, %edi
		movl 	$p3_table, %eax
		orl 	$0b11, %eax
		movl 	%eax, (%edi)

		// p3 -> p2
		movl    $p3_table, %edi
		movl 	$p2_table, %eax
		orl 	$0b11, %eax
		movl 	%eax, (%edi)

		// p2 -> p1
		movl    $p2_table, %edi
		movl 	$p1_table, %eax
		orl 	$0b11, %eax
		movl 	%eax, (%edi)


		movl 	$0, %ecx
		.identity_map:
			movl	$0x1000, %eax  // 4k pages
			mull 	%ecx
			orl 	$0b11, %eax
			movl	%eax, p1_table(,%ecx,8)
			inc 	%ecx
			cmp 	$512, %ecx
			jne		.identity_map


	.switch_to_compatability_mode:

		.Lset_page_table:
		movl 	$p4_table, %eax
		movl	%eax, %cr3


		.Lenable_pae_paging:
		movl	%cr4, %eax
		orl 	$PAE_PAGING_BIT, %eax
		movl	%eax, %cr4


		// set EFER MSR and LM-bit
		movl	$EFER_MSR, %ecx
		rdmsr
		orl 	$LM_BIT, %eax
		wrmsr

		// enable paging
		movl	%cr0, %eax
		orl 	$PAGING_BIT, %eax
		movl	%eax, %cr0


hlt


	.load_global_descriptor_table:
		lgdt 	GDT64

		// .Lrefresh_segments:
		movw 	$0, %ax
		movw	%ax, %ds
		movw	%ax, %es
		movw	%ax, %fs
		movw	%ax, %gs
		movw	%ax, %ss




	.Lmain64:
		# initialize stack
		// movl $stack_top, %esp
		// movl $stack_top, %ebp
		// call main
		// jmp main64
		ljmp	$8, $main64




_end:
	cli
	hlt
.Lhang:
	jmp .Lhang

.size _start, . - _start



.section .rodata

	.set GDT_EXECUTABLE, 1<<43
	.set GDT_CODE_SEGMENT, 1<<44
	.set GDT_PRESENT, 1<<47
	.set GDT_64BIT, 1<<53

	GDT:
		.null:
			.quad 0
		.code:
			.quad (GDT_EXECUTABLE | GDT_CODE_SEGMENT | GDT_PRESENT | GDT_64BIT)
	GDT_end:

	GDT64:
		.word (GDT_end - GDT - 1)
		.quad GDT





# 16kb stack
.section .bootstrap_stack, "aw", @nobits

	stack_bottom:
		.skip 16384 
	stack_top:


.section .bss
.align 4096
p4_table:
.align 4096
	.skip 4096
p3_table:
.align 4096
	.skip 4096
p2_table:
.align 4096
	.skip 4096
p1_table:
.align 4096
	.skip 4096
