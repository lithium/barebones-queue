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

	self->rsdp1 = (struct acpi_rsdp1 *)rsdp;
	self->rsdt = (struct acpi_rsdt *)self->rsdp1->rsdtAddress;

	self->rsdtEntryCount = (self->rsdt->header.length - sizeof(struct acpi_sdt_header)) / 4;

	for (int i = 0; i < self->rsdtEntryCount; i++) {
		struct acpi_sdt_header *header = (struct acpi_sdt_header *)(self->rsdt->entries[i]);

		if (StringEq(header->signature, "FACP", 4)) {
			self->fadt = (struct acpi_fadt *)header;
		}
		else if (StringEq(header->signature, "APIC", 4)) {
			self->madt = (struct acpi_madt *)header;

			self->madtProcessorEntryCount = 0;
			self->madtIoApicEntryCount = 0;
			int ofs = 44;
			while (ofs < header->length) {
				struct acpi_madt_header *apic_header = (struct acpi_madt_header *)((uint8_t*)self->madt + ofs);

				if (apic_header->type == MADT_APIC_PROCESSOR_LOCAL_APIC) {
					self->madtProcessorEntries[self->madtProcessorEntryCount] = (struct acpi_madt_processor_local_apic *)apic_header;
					self->madtProcessorEntryCount += 1;
				}
				else if (apic_header->type == MADT_APIC_IO_APIC) {
					self->madtIoApicEntries[self->madtIoApicEntryCount] = (struct acpi_madt_io_apic *)apic_header;
					self->madtIoApicEntryCount += 1;
				}


				ofs += apic_header->length;
			}
		}
	}

	return 0;
}

void AcpiInfo_print(AcpiInfo *self)
{
	char buf[17];

	Print("  rsdp sig='");
	Printn(self->rsdp1->signature, 8);

	Print("' oemid='");
	Printn(self->rsdp1->oemId, 6);

	Print("' revision=0x");
	Println(Hexstring(buf, 16, self->rsdp1->revision));

	Print("  rsdt=0x");
	Print(Hexstring(buf, 16, self->rsdp1->rsdtAddress));

	Print(" entryCount=0x");
	Println(Hexstring(buf, 16, self->rsdtEntryCount));

	for (int i = 0; i < self->rsdtEntryCount; i++) {
		struct acpi_sdt_header *header = (struct acpi_sdt_header *)(self->rsdt->entries[i]);

		Print("    rsdt sig='");
		Printn(header->signature,4);
		Print("' oemId='");
		Printn(header->oemId,6);
		Print("' oemTableId='");
		Printn(header->oemTableId,8);
		Println("'");
	}


	Print("Number of Processors: 0x");
	Println(Hexstring(buf, 16, self->madtProcessorEntryCount));
	for (int i=0; i < self->madtProcessorEntryCount; i++) {
		Print("  APIC ID: ");
		Println(Hexstring(buf, 16, self->madtProcessorEntries[i]->apicId));
	}

	for (int i=0; i < self->madtIoApicEntryCount; i++) {
		Print("I/O APIC id=0x");
		Print(Hexstring(buf,16, self->madtIoApicEntries[i]->ioApicId));
		Print(" addr=0x");
		Println(Hexstring(buf,16, self->madtIoApicEntries[i]->ioApicAddress));
	}

}