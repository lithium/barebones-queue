#include <stdint.h>
#include "vga/vga.h"

#include "string/String.h"
#include "multiboot/Multiboot2Info.h"
#include "acpi/acpi.h"


extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	Multiboot2Info mb2info;
	AcpiInfo acpiInfo;

	Multiboot2Info_create(&mb2info, multiboot2_info_addr);
	Multiboot2Info_print(&mb2info);

	if (mb2info.oldAcpiTag) {
		AcpiInfo_create(&acpiInfo, mb2info.oldAcpiTag->rsdp);
		AcpiInfo_print(&acpiInfo);
	}

}