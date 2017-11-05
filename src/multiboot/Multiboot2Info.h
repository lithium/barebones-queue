#ifndef BAREBONES_MULTIBOOT_H
#define BAREBONES_MULTIBOOT_H

#include <inttypes.h>

#include "multiboot/multiboot2.h"

#define MAX_MEMORY_MAP_SIZE 99

typedef struct multiboot2_information {
	uint64_t physicalAddress;
	uint32_t size;


	struct multiboot_tag_string *commandLineTag;
	struct multiboot_tag_string *bootLoaderNameTag;

	struct multiboot_tag_basic_meminfo *basicMeminfoTag;
	struct multiboot_tag_bootdev *bootDeviceTag;

	struct multiboot_tag_mmap *memoryMapTag;

	struct multiboot_tag_new_acpi *newAcpiTag;
	struct multiboot_tag_old_acpi *oldAcpiTag;

	struct multiboot_tag_smbios *smbiosTag;

	struct multiboot_tag_elf_sections *elfSectionsTag;


	// TODO: unimplemented
	// struct multiboot_tag_module *moduleTag;
	// struct multiboot_tag_framebuffer *framebufferTag;
} Multiboot2Info;


void Multiboot2Info_create(Multiboot2Info *self, uint64_t multiboot2_info_addr);

#endif