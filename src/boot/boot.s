# Declare constants used for creating a multiboot header.
.set MULTIBOOT_ALIGN, 1<<0          # align loaded modules on page boundaries
.set MULTIBOOT_MEMINFO, 1<<1 		# provide memory map
.set MULTIBOOT_FLAGS, MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO  
.set MULTIBOOT_MAGIC, 0x1BADB002       
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) 

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM


# 16kb stack
.section .bootstrap_stack, "aw", @nobits

stack_bottom:
	.skip 16384 
stack_top:


.section .text


# _start() 
#
#  
.global _start
.type _start, @function
_start:
	# initialize stack
	movl $stack_top, %esp

	call main

	# deadlock
	cli
	hlt
.Lhang:
	jmp .Lhang

.size _start, . - _start


