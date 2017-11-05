#ifndef BAREBONES_ACPI_MADT_H
#define BAREBONES_ACPI_MADT_H


// #define MADT_APIC_PROCESSOR_LOCAL_APIC 	0
// #define MADT_APIC_LOCAL_APIC 			1
// #define MADT_APIC_INTERRUPT_SOURCE_OVERRIDE 			2
// #define MADT_APIC_NMI_SOURCE 			3
// #define MADT_APIC_LOCAL_APIC_NMI 			4
// #define MADT_APIC_LOCAL_APIC_ADRESS_OVERRIDE 			5
// #define MADT_APIC_IO_SAPIC 			5
// #define MADT_APIC_LOCAL_SAPIC 			5
// #define MADT_APIC_PLATFORM_INTERRUPT_SOURCES 			5


struct acpi_madt_header {
	uint8_t type;
	uint8_t length;
};


struct acpi_madt {
    struct acpi_sdt_header header;

    uint32_t LocalInterruptControllerAddress;
    uint32_t Flags;
} __attribute__((packed));

#endif
