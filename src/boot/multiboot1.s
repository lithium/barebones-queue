
# multiboot1

.section .multiboot
.align 4

	# .set MULTIBOOT_ALIGN, 1<<0          # align loaded modules on page boundaries
	# .set MULTIBOOT_MEMINFO, 1<<1 		# provide memory map
	# .set MULTIBOOT_FLAGS, MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO  
	# .set MULTIBOOT_MAGIC, 0x1BADB002       
	# .set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) 

	# .multiboot_header:
	# 	.long MULTIBOOT_MAGIC
	# 	.long MULTIBOOT_FLAGS
	# 	.long MULTIBOOT_CHECKSUM




.multiboot_header:

	.set MB2_MAGIC, 0xe85250d6
	.set MB2_ARCH, 0
	.set MB2_SIZE, .multiboot_header_end - .multiboot_header
	.set MB2_CHKSUM, 0x100000000 - (MB2_MAGIC + MB2_ARCH + MB2_SIZE)

		.long MB2_MAGIC
		.long MB2_ARCH
		.long MB2_SIZE
		.long MB2_CHKSUM

		// end tag
		.word 0
		.word 0
		.long 8

.multiboot_header_end: