#ifndef BAREBONES_APIC_H
#define BAREBONES_APIC_H



#define APIC_ACR_DELIVERY_MODE_FIXED  (0<<8)
#define APIC_ACR_DELIVERY_MODE_LOW_PRIORITY  (1<<8)
#define APIC_ACR_DELIVERY_MODE_SMI  (2<<8)
#define APIC_ACR_DELIVERY_MODE_NMI  (4<<8)
#define APIC_ACR_DELIVERY_MODE_INIT  (5<<8)
#define APIC_ACR_DELIVERY_MODE_STARTUP  (6<<8)

#define APIC_ACR_DESTINATION_MODE_PHYSICAL (0<<11)
#define APIC_ACR_DESTINATION_MODE_LOGICAL (1<<11)

#define APIC_ACR_DEST_SHORTHAND_NONE (0<<18)
#define APIC_ACR_DEST_SHORTHAND_SELF (1<<18)
#define APIC_ACR_DEST_SHORTHAND_ALL (2<<18)
#define APIC_ACR_DEST_SHORTHAND_ALL_EXCEPT_SELF (3<<18)

#define APIC_ACR_LEVEL_DEASSERT (0<<14)
#define APIC_ACR_LEVEL_ASSERT (1<<14)
#define APIC_ACR_TRIGGER_EDGE (0<<15)
#define APIC_ACR_TRIGGER_LEVEL (1<<15)


uint32_t apic_mmio_read(uint64_t base, unsigned int reg);
void apic_mmio_write(uint64_t base, unsigned int reg, uint32_t val);

uint32_t apic_base_address();

#endif
