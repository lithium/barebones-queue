#include <inttypes.h>
#include "vga/vga.h"

#include "string/String.h"
#include "multiboot/Multiboot2Info.h"
#include "acpi/acpi.h"
#include "apic/apic.h"
#include "arch/x86.h"
#include "arch/x86/idt.h"


extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;

#define reg32_offset(base, regoffset)   (*(uint32_t *)(((uint8_t*)(base)) + (regoffset)))
#define reg64_offset(base, regoffset)   (*(uint64_t *)(((uint8_t*)(base)) + (regoffset)))


void main64() 
{
	char hexbuf[17]; // debug buffer

	Multiboot2Info mb2info;
	AcpiInfo acpiInfo;

	// load interrupts
	IdtLoad();

	// parse multiboot2 information
	Multiboot2Info_create(&mb2info, multiboot2_info_addr);
	Multiboot2Info_print(&mb2info);

	// parse acpi information
	if (mb2info.oldAcpiTag) {
		AcpiInfo_create(&acpiInfo, mb2info.oldAcpiTag->rsdp);
		AcpiInfo_print(&acpiInfo);
	}

	uint32_t *ioapic_base = acpiInfo.madtIoApicEntries[0]->ioApicAddress;


	// recursively mapped page table pointers
	uint64_t *P4 = 0xfffffffffffff000;		// P4[0] 1G identity map
	uint64_t *P3 = (uint64_t)P4<<9; 		// P3[0] 1G identity map, P3[1] apic mmio 
	uint64_t *P2 = (uint64_t)P3<<9; 		// P2[*] 1G identity map,
	uint64_t *apic_page = (uint64_t)P2 + 0x1000;	// apic mmio

	// map 2MB for apic base at 0x40000000
	apic_page[0] = (uint32_t)ioapic_base | 0x83;	// present | writable | 2MB
	uint32_t virtual_apic_base = *(uint32_t *)0x40000000;


	//disable PIC
	OUTB(0xa1, 0xFF);
	OUTB(0x21, 0xFF);

	// enable local io apic
	reg32_offset(virtual_apic_base, 0x0F0) |= 0x10F;

	// send INIT/STARTUP IPI to all-except-self
	reg32_offset(virtual_apic_base, 0x300) = APIC_ACR_DELIVERY_MODE_STARTUP
						 | APIC_ACR_DEST_SHORTHAND_ALL_EXCEPT_SELF 
						 | 0x91 // realmode entry at 0x91000
						 ;

	reg32_offset(virtual_apic_base, 0x300) = APIC_ACR_DELIVERY_MODE_INIT 
						 | APIC_ACR_DEST_SHORTHAND_ALL_EXCEPT_SELF 
						 ;

	Println("Yay!");
	for(;;);
}



