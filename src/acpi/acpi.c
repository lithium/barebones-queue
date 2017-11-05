#include "acpi/acpi.h"
#include "string/String.h"
#include "vga/vga.h"

int AcpiValidateChecksum(void *header, int len)
{
	uint8_t sum = 0;
	while (len--) {
		sum += *(uint8_t*)(header+len);
	}
	return sum == 0;
}


int AcpiInfo_create(AcpiInfo *self, void *rsdp)
{
	Memset(self, 0, sizeof(AcpiInfo));
	char buf[17];


	self->rsdp1 = (struct acpi_rsdp1 *)rsdp;
	Print("  rsdp sig='");
	Printn(self->rsdp1->signature, 8);

	Print("' oemid='");
	Printn(self->rsdp1->oemId, 6);

	Print("' revision=0x");
	Println(Hexstring(buf, 16, self->rsdp1->revision));

	Print("  rsdt=0x");
	Print(Hexstring(buf, 16, self->rsdp1->rsdtAddress));

	// if (self->rsdp1->revision != 1) {
		// return -1;
	// }

	self->rsdt = (struct acpi_rsdt *)self->rsdp1->rsdtAddress;
	int num_entries = (self->rsdt->header.length - sizeof(struct acpi_sdt_header)) / 4;

	Print(" numEntries=0x");
	Println(Hexstring(buf, 16, num_entries));

	for (int i = 0; i < num_entries; i++) {
		struct acpi_sdt_header *header = (struct acpi_sdt_header *)(self->rsdt->entries[i]);

		Print("    rsdt sig='");
		Printn(header->signature,4);
		Print("' oemId='");
		Printn(header->oemId,6);
		Print("' oemTableId='");
		Printn(header->oemTableId,8);
		Println("'");

		if (StringEq(header->signature, "FACP", 4)) {
			self->fadt = (struct acpi_fadt *)header;
		}
		else if (StringEq(header->signature, "APIC", 4)) {
			self->madt = (struct acpi_madt *)header;

			int ofs = 44;
			while (ofs < header->length) {
				struct acpi_madt_header *apic_header = (struct acpi_madt_header *)((uint8_t*)self->madt + ofs);

				Print("      type=0x");
				Println(Hexstring(buf,16, apic_header->type & 0xFF));

				ofs += apic_header->length;

			}
		}

			
	}

	return 0;
}