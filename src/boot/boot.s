
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

	.Lok:
		movl	$0xb8000, %edi
		movl	$0x4f4b4f4f, (%edi)



	.Lmain:
		# initialize stack
		// movl $stack_top, %esp
		// movl $stack_top, %ebp
		// call main




_end:
	cli
	hlt
.Lhang:
	jmp .Lhang

.size _start, . - _start


# 16kb stack
.section .bootstrap_stack, "aw", @nobits

	stack_bottom:
		.skip 16384 
	stack_top:
