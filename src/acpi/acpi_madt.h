#ifndef BAREBONES_ACPI_MADT_H
#define BAREBONES_ACPI_MADT_H


#define MADT_APIC_PROCESSOR_LOCAL_APIC 	0
#define MADT_APIC_LOCAL_APIC 			1
#define MADT_APIC_INTERRUPT_SOURCE_OVERRIDE 			2
#define MADT_APIC_NMI_SOURCE 			3
#define MADT_APIC_LOCAL_APIC_NMI 			4



struct acpi_madt {
    struct acpi_sdt_header header;

    uint32_t LocalInterruptControllerAddress;
    uint32_t Flags;
} __attribute__((packed));

struct acpi_madt_header {
	uint8_t type;
	uint8_t length;
} __attribute__((packed));


struct acpi_madt_processor_local_apic {
	struct acpi_madt_header header;

	uint8_t acpiProcessorId;
	uint8_t apicId;
	uint8_t flags;
} __attribute__((packed));

struct acpi_madt_io_apic {
	struct acpi_madt_header header;

	uint8_t ioApicId;
	uint8_t reserved;
	uint32_t ioApicAddress;
	uint32_t globalSystemInterruptBase;
} __attribute__((packed));

struct acpi_madt_interrupt_source_override {
	struct acpi_madt_header header;

	uint8_t bus;
	uint8_t source;
	uint32_t globalSystemInterrupt;
	uint16_t flags;
} __attribute__((packed));


struct acpi_madt_local_apic_nmi {
	struct acpi_madt_header header;

	uint8_t acpiProcessorId;
	uint16_t flags;
	uint8_t localApicLint;
} __attribute__((packed));
#endif
