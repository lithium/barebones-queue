#include <stdint.h>
#include "vga/vga.h"

#include "string/String.h"
#include "multiboot/Multiboot2Info.h"
#include "acpi/acpi.h"


extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	char hexstring[17];
	hexstring[16] = 0;

	// for (int i=0xFFFFFFFFF<<12; i <= -1; i++) {
		// Println(Hexstring(hexstring,16, i));
	// }

	Multiboot2Info mb2info;
	Multiboot2Info_create(&mb2info, multiboot2_info_addr);

	Print("Bootloader name: ");
	Println(mb2info.bootLoaderNameTag->string);

	Print("Commandline: ");
	Println(mb2info.commandLineTag->string);

	if (mb2info.elfSectionsTag) {
		Print("ELF Sections: count=0x");
		Println(Hexstring(hexstring, 16, mb2info.elfSectionsTag->num));
	}

	if (mb2info.basicMeminfoTag) {
		Print("Basic Meminfo: lower=0x");
		Print(Hexstring(hexstring, 16, mb2info.basicMeminfoTag->mem_lower));
		Print(" upper=0x");
		Println(Hexstring(hexstring, 16, mb2info.basicMeminfoTag->mem_upper));
	}

	if (mb2info.memoryMapTag) {
		int entry_count = mb2info.memoryMapTag->size / mb2info.memoryMapTag->entry_size;
		Print("Memory Map: numEntries=0x");
		Println(Hexstring(hexstring,16, entry_count));
		for (int i=0; i < entry_count; i++) {
			multiboot_memory_map_t *mmap = ((uint8_t*)mb2info.memoryMapTag->entries + (i * mb2info.memoryMapTag->entry_size));
			Print("  addr=0x");
			Print(Hexstring(hexstring,16, mmap->addr));
			Print(" len=0x");
			Print(Hexstring(hexstring,16, mmap->len));
			Print(" type=0x");
			Println(Hexstring(hexstring,16, mmap->type));
		}
	}

	if (mb2info.oldAcpiTag) {
		Print("old ACPI: RSDPv1 size=0x");
		Println(Hexstring(hexstring, 16, mb2info.oldAcpiTag->size));

		AcpiInfo acpiInfo;
		AcpiInfo_create(&acpiInfo, mb2info.oldAcpiTag->rsdp);
	}

	if (mb2info.newAcpiTag) {
		Print("New ACPI: RSDPv2 size=0x");
		Println(Hexstring(hexstring, 16, mb2info.newAcpiTag->size));
	}

}