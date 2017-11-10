#include <inttypes.h>
#include "vga/vga.h"

#include "string/String.h"
#include "multiboot/Multiboot2Info.h"
#include "acpi/acpi.h"
#include "arch/x86.h"
#include "arch/x86/idt.h"


extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	Multiboot2Info mb2info;
	AcpiInfo acpiInfo;

	// load interrupts
	IdtLoad();

	Multiboot2Info_create(&mb2info, multiboot2_info_addr);
	Multiboot2Info_print(&mb2info);

	if (mb2info.oldAcpiTag) {
		AcpiInfo_create(&acpiInfo, mb2info.oldAcpiTag->rsdp);
		AcpiInfo_print(&acpiInfo);
	}

	uint32_t *ioapic_base = acpiInfo.madtIoApicEntries[0]->ioApicAddress;

#define reg32_offset(base, regoffset)   (*(uint32_t *)(((uint8_t*)(base)) + (regoffset)))
#define reg64_offset(base, regoffset)   (*(uint64_t *)(((uint8_t*)(base)) + (regoffset)))

	uint16_t f = *(uint16_t *)0x3FFFFFFE;


	char buf[17];

	uint64_t *P4 = 0xfffffffffffff000;
	uint64_t *P3 = (uint64_t)P4<<9;
	uint64_t *P2 = (uint64_t)P3<<9;

	Print("P4[511]: 0x");
	Println(Hexstring(buf,16, P4[511]));
	Print("P4[0]: 0x");
	Println(Hexstring(buf,16, P4[0]));
	Print("P3[0]: 0x");
	Println(Hexstring(buf,16, P3[0]));
	Print("P2[0]: 0x");
	Println(Hexstring(buf,16, P2[0]));

	Print("P3[1]: 0x");
	Println(Hexstring(buf,16, P3[1]));

	//page table
	// 00003FFF FFFF
	// uint64_t *apic_page = ((uint64_t)P3<<9) + 0x1000;
	// Print("apic_page: 0x");
	// Println(Hexstring(buf,16, apic_page));

	// apic_page[0] = (((uint64_t)0xb8000)<<21)|0x83;

	uint32_t virtual_apic_base = *(uint32_t *)0x40000000;




	//map ioapic into virtual memory
	// physical address=0x000 fec0 0000


	//disable PIC
	OUTB(0xa1, 0xFF);
	OUTB(0x21, 0xFF);

	// enable local io apic
	reg32_offset(virtual_apic_base, 0x0F0) |= 0x100;
	// ioapic_base[0x0F0/4] |= 0x100;

	// send INIT/STARTUP IPI to all-except-self
	// ioapic_base[0x310] = dest;
	// ioapic_base[0x300] = 

	Println("Yay!");
}



