#ifndef BAREBONES_ACPI_H
#define BAREBONES_ACPI_H

#include <inttypes.h>


struct acpi_rsdp1 {
	char signature[8];
	uint8_t checksum;
	char oemId[6];
	uint8_t revision;
	uint32_t rsdtAddress;
} __attribute__ ((packed));

struct acpi_rsdp2 {
	struct acpi_rsdp1 header;

	uint32_t length;
	uint64_t xstAddress;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
} __attribute__ ((packed));


struct acpi_sdt_header {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemId[6];
	char oemTableId[8];
	uint32_t oemRevision;
	uint32_t creatorId;
	uint32_t creatorRevision;
} __attribute__ ((packed));

struct acpi_rsdt {
	struct acpi_sdt_header header;
	uint32_t entries[0];
} __attribute__ ((packed));


struct acpi_address
{
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
} __attribute__ ((packed));


#include "acpi_fadt.h"
#include "acpi_madt.h"


typedef struct AcpiInfo {
	struct acpi_rsdp1 *rsdp1;
	struct acpi_rsdt *rsdt;

	struct acpi_fadt *fadt;
	struct acpi_madt *madt;
} AcpiInfo;


int AcpiInfo_create(AcpiInfo *self, void *rsdp);

int AcpiValidateChecksum(void *header, int len);

#endif