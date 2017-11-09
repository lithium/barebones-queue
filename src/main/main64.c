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

	Multiboot2Info_create(&mb2info, multiboot2_info_addr);
	Multiboot2Info_print(&mb2info);

	if (mb2info.oldAcpiTag) {
		AcpiInfo_create(&acpiInfo, mb2info.oldAcpiTag->rsdp);
		AcpiInfo_print(&acpiInfo);
	}



	uint32_t *ioapic_base = acpiInfo.madtIoApicEntries[0]->ioApicAddress;

	//set apic base to 0x300000


	//disable PIC
	// OUTB(0xa1, 0xFF);
	// OUTB(0x21, 0xFF);

	// enable local io apic
	// ioapic_base[0x0F0] |= 0x100;

	// send INIT/STARTUP IPI to all-except-self
	// ioapic_base[0x310] = dest;
	// ioapic_base[0x300] = 



	// IdtSetGate(0, interrupt_handler, 0x8, 0x8E);
	IdtLoad();


	// force a divide by zero
	int foo = 5;
	int bar = foo / 0;
	Println("hih");
}



