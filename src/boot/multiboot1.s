
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

