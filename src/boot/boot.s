.global start
.global multiboot2_info_addr
.global multiboot2_magic

.section .bss

multiboot2_magic:
		.long	0

multiboot2_info_addr:
		.long	0



.section .realmode
.code16

real_to_longmode:
		movl	$0xb8000, %di
		movl	$0x700, %ax		// clear screen to be gray on black
		movl 	$(80*25), %cx
	rep stosw
		hlt


.section .text
.code32


start:
		//arrived from multiboot in protected mode

		// save multiboot information for kernel
		movl	%eax, (multiboot2_magic)
		movl	%ebx, (multiboot2_info_addr)

		// initialize stack
		movl 	$stack_top, %esp 

		call	clear_screen
		// cleared screen indicates we booted

		call	protected_to_longmode


	.Lmain:
		lgdt	GDT.pointer

		// 8 is the offset in GDT to the code segment
		ljmp	$8, $longmode_start

longmode_start:
.code64
		mov	$0, %ax
		mov	%ax, %ss
		mov	%ax, %ds
		mov	%ax, %es
		mov	%ax, %fs
		mov	%ax, %gs

		call	main64

		// returned from main
		movq	$0x2f542f4c2f412f48, %rax
		movq	%rax, (0xb8f98)
		jmp 	halt




.code32


/*
 * error - print ERRN to vga mem
 * 		%al: error code in ascii
 */
error:
		movl	$0xb8000, %edi
		movl	$0x4f524f45, (%edi)
		add 	$4, %edi
		movl	$0x4f004f52, (%edi)
		add 	$4, %edi
		movl	$0x4f204f20, (%edi)
		movb 	$0x4f, %ah
		movw 	%ax, (%edi)


halt:
		cli
		hlt
	.Lhang:
		jmp .Lhang


/*
 * clear_screen
 * 
 */
clear_screen:
		movl	$0xb8000, %edi
		movl	$0x700, %eax		// clear screen to be gray on black
		movl 	$(80*25), %ecx
	rep stosw
		ret


protected_to_longmode:
		call	check_cpuid
		call	check_longmode
		call	setup_pagetables
		call	enable_paging
		ret

check_cpuid:
	.set CPUID_BIT,  1<<21

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
		jz		.Lno_cpuid
		ret
	.Lno_cpuid:
		movb	$'C', %al
		jmp 	error


check_longmode:
	.set LONGMODE_BIT, 1<<29

		// check for extended cpuid 
		movl 	$0x80000000, %eax
		cpuid
		cmp		$0x80000001, %eax
		jb 		.Lno_longmode

		// check for longmode
		movl 	$0x80000001, %eax
		cpuid
		test 	$LONGMODE_BIT, %edx
		jz 		.Lno_longmode
		ret
	.Lno_longmode:
		movb	$'L', %al
		jmp		error



setup_pagetables:

		// p4[511] -> p4 
		mov	$p4_table, %eax
		or 	$0b11, %eax // present + writable
		mov 	%eax, (p4_table+511*8)

		// p4[0] -> p3
		mov 	$p3_table, %eax
		or 	$0b11, %eax
		mov 	%eax, (p4_table)

		// p3[0] -> p2
		mov 	$p2_table, %eax
		or 	$0b11, %eax
		mov 	%eax, (p3_table)

		// p3[1] -> apic_table
		mov	$apic_table, %eax
		or	$0b11, %eax
		mov	%eax, (p3_table+1*8)

		// identity map first 1G
		mov	$0, %ecx
	.Lidentity_map_p2:
		mov	$0x200000, %eax 			// 2MB pages
		mul 	%ecx
		or 	$0b10000011, %eax			// huge + present + writable
		mov 	%eax, p2_table(, %ecx, 8)

		inc 	%ecx
		cmp     $512, %ecx
		jne 	.Lidentity_map_p2

		ret


enable_paging:
	.set PAE_PAGING_BIT, 1<<5
	.set EFER_MSR, 0xC0000080
	.set LM_BIT, 1<<8
	.set PAGING_BIT, 1<<31

		// %cr3 = p4_table
		movl 	$p4_table, %eax
		movl	%eax, %cr3

		// enable PAE
		movl	%cr4, %eax
		orl 	$PAE_PAGING_BIT, %eax
		movl	%eax, %cr4

		// set long mode bit in EFER MSR
		movl	$EFER_MSR, %ecx
		rdmsr
		orl 	$LM_BIT, %eax
		wrmsr

		// enable paging
		movl	%cr0, %eax
		orl 	$PAGING_BIT, %eax
		movl	%eax, %cr0
		ret


.section .text




.section .rodata

	.set GDT_EXECUTABLE, 1<<43
	.set GDT_CODE_SEGMENT, 1<<44
	.set GDT_PRESENT, 1<<47
	.set GDT_64BIT, 1<<53

GDT:
		.quad	0 	// null entry
	.code:
		.quad (GDT_EXECUTABLE | GDT_CODE_SEGMENT | GDT_PRESENT | GDT_64BIT)

GDT.pointer:
		.word (. - GDT - 1)
		.quad GDT



// # 16kb stack
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

apic_table:
.align 4096
	.skip 4096
