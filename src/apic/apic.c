#include <inttypes.h>
#include "apic.h"
#include "arch/x86.h"


#ifndef _p
#define _p(a)   ((uint8_t*)a)
#endif

inline uint32_t apic_mmio_read(uint64_t base, unsigned int reg)
{
	return *(volatile uint32_t *)(_p(base) + reg);
}

inline void apic_mmio_write(uint64_t base, unsigned int reg, uint32_t val)
{
	*(volatile uint32_t *)(_p(base) + reg) = val;
}

inline uint32_t apic_base_address()
{
	uint64_t value = RDMSR(0x1b);
	return (value & 0xFFFFF000);
}
