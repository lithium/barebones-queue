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

			case MULTIBOOT_TAG_TYPE_MMAP: {
				multiboot_memory_map_t *mmap;

				self->memoryMapTag = (struct multiboot_tag_mmap *)tag;


				self->memoryMapSize = 0;
				mmap = self->memoryMapTag->entries;
				while ((uint8_t*)mmap < (uint8_t*)tag+tag->size) {
					self->memoryMap[self->memoryMapSize] = mmap;

					mmap += self->memoryMapTag->entry_size;
					self->memoryMapSize += 1;

					if (self->memoryMapSize >= MAX_MEMORY_MAP_SIZE)	{
						break;
					}
				}


				break;
			}

		}


		tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7));
	}
}