#include <inttypes.h>
#include "vga/vga.h"

#include "string/String.h"
#include "multiboot/Multiboot2Info.h"
#include "acpi/acpi.h"
#include "apic/apic.h"
#include "arch/x86.h"
#include "arch/x86/idt.h"
#include "pit/pit.h"


extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;

#define reg32_offset(base, regoffset)   (*(uint32_t *)(((uint8_t*)(base)) + (regoffset)))
#define reg64_offset(base, regoffset)   (*(uint64_t *)(((uint8_t*)(base)) + (regoffset)))






void main64() 
{
	// recursively mapped page table pointers
	uint64_t *P4 = 0xfffffffffffff000;		// P4[0] 1G identity map
	uint64_t *P3 = (uint64_t)P4<<9; 		// P3[0] 1G identity map, P3[1] apic mmio 
	uint64_t *P2 = (uint64_t)P3<<9; 		// P2[*] 1G identity map,
	uint64_t *apic_page = ((uint64_t)P3<<9) + 0x1000;	// apic mmio

	char hexbuf[17]; // debug buffer
	Memset(hexbuf, 0, 17);



	Multiboot2Info mb2info;
	AcpiInfo acpiInfo;

	// parse multiboot2 information
	Multiboot2Info_create(&mb2info, multiboot2_info_addr);
	Multiboot2Info_print(&mb2info);

	// parse acpi information
	if (mb2info.oldAcpiTag) {
		AcpiInfo_create(&acpiInfo, mb2info.oldAcpiTag->rsdp);
		AcpiInfo_print(&acpiInfo);
	}

	// load interrupts
	IdtLoad();

	//setup PIC/PIT
	PicRemap(32);
	OUTB(PORT_PIT_MODE, 0x36); 	// ch=0 access=lobyte/hibyte operating=rate bcd=0
	PitSetFrequency(5000);  	// each tick is 200us

	// start interrupts
	STI();

	// uint32_t *ioapic_base = acpiInfo.madtIoApicEntries[0]->ioApicAddress;
	uint32_t *ioapic_base = apic_base_address();

	Print("MSR APIC base address: 0x");
	Println(Hexstring(hexbuf,16, (uint32_t)ioapic_base));

	// map 2MB for apic base at 0x40000000
	apic_page[0] = (uint32_t)ioapic_base | 0x83;	// present | writable | 2MB
	uint32_t virtual_apic_base = (uint32_t *)0x40000000;

	Print("Local APIC Version: 0x");
	Println(Hexstring(hexbuf,16, apic_mmio_read(virtual_apic_base, 0x30)));

	uint16_t *processor_count = 0x9000;
	*processor_count = 1;

	// enable local io apic with spurious vector=FF
	apic_mmio_write(virtual_apic_base, 0x0F0, 0x1FF);

	#define INIT_ICR (APIC_ACR_DELIVERY_MODE_INIT | APIC_ACR_DEST_SHORTHAND_ALL_EXCEPT_SELF | APIC_ACR_LEVEL_ASSERT)
	#define STARTUP_ICR (APIC_ACR_DELIVERY_MODE_STARTUP | APIC_ACR_DEST_SHORTHAND_ALL_EXCEPT_SELF | APIC_ACR_LEVEL_ASSERT)

	// send INIT/STARTUP IPI to all-except-self
	apic_mmio_write(virtual_apic_base, 0x310, 0);
	apic_mmio_write(virtual_apic_base, 0x300, INIT_ICR);

	// sleep for 10ms
	PicSleepTicks(50);

	apic_mmio_write(virtual_apic_base, 0x310, 0);
	apic_mmio_write(virtual_apic_base, 0x300, STARTUP_ICR | 0x91);

	// sleep for 200us
	PicSleepTicks(1);

	apic_mmio_write(virtual_apic_base, 0x310, 0);
	apic_mmio_write(virtual_apic_base, 0x300, STARTUP_ICR | 0x91); 


	Println("Waiting for processors...");
	while (*processor_count < acpiInfo.madtProcessorEntryCount) {

	}

	Print("Processors woke: 0x");
	Println(Hexstring(hexbuf,16, *processor_count));

	for(;;);
}


