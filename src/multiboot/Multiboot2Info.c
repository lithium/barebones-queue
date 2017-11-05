#include "multiboot/Multiboot2Info.h"
#include "string/String.h"


void Multiboot2Info_create(Multiboot2Info *self, uint64_t addr)
{
	Memset(self, 0, sizeof(Multiboot2Info));

	self->physicalAddress = addr;
	self->size = *(uint32_t *)addr;

	struct multiboot_tag *tag = (struct multiboot_tag *)(addr+8); 

	while (tag->type != MULTIBOOT_TAG_TYPE_END) {
		switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				self->commandLineTag = (struct multiboot_tag_string *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				self->bootLoaderNameTag = (struct multiboot_tag_string *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				self->basicMeminfoTag = (struct multiboot_tag_basic_meminfo *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				self->bootDeviceTag = (struct multiboot_tag_bootdev *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_MMAP: 
				self->memoryMapTag = (struct multiboot_tag_mmap *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_ACPI_NEW: 
				self->newAcpiTag = (struct multiboot_tag_new_acpi *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_ACPI_OLD: 
				self->oldAcpiTag = (struct multiboot_tag_old_acpi *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_SMBIOS: 
				self->smbiosTag = (struct multiboot_tag_smbios *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_ELF_SECTIONS: 
				self->elfSectionsTag = (struct multiboot_tag_elf_sections *)tag;
				break;

		}


		tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7));
	}
}