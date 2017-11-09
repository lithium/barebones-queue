#ifndef BAREBONES_ARCH_X86_IDT_H
#define BAREBONES_ARCH_X86_IDT_H

#include <inttypes.h>
#include "arch/x86.h"


struct idt_info {
	uint16_t length;
	uintptr_t base;
} __attribute__((packed));

struct idt_gate {
	uint16_t offsetLow;
	uint16_t codeSelector;
	uint8_t zero;
	uint8_t flags;
	uint16_t offsetMiddle;
	uint32_t offsetHigh;
	uint32_t reserved;
} __attribute__((packed));


struct interrupt_frame
{
	uint64_t cr2;
	
	uint64_t number;
	uint64_t error_code, rip, cs, rflags, rsp, ss;
} __attribute__((packed));


extern struct idt_gate IDT64[256];
extern struct idt_info IDT64_info;

void IdtSetGate(uint8_t num, uint64_t offset, uint16_t selector, uint8_t flags);
void IdtLoad();

#endif