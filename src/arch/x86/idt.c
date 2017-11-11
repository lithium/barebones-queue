#include "arch/x86/idt.h"
#include "vga/vga.h"


struct idt_gate IDT64[256];
struct idt_info IDT64_info;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isrFF();
void (*EXCEPTIONS[])() = {
	isr0,
	isr1,
	isr2,
	isr3,
	isr4,
	isr5,
	isr6,
	isr7,
	isr8,
	isr9,
	isr10,
	isr11,
	isr12,
	isr13,
	isr14,
	isr15,
	isr16,
	isr17,
	isr18,
	isr19,
	isr20,
};


char *EXCEPTION_NAMES[] = {
	"Divide by 0",
	"Debug Exception",
	"NMI Interrupt",
	"Breakpoint Exception",
	"Overflow Exception",
	"BOUND Exception",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Exception",
	"Page Fault",
	"Floating Point Exception",
	"Alignment Check Exception",
	"Machine-Check Exception",
	"SIMD Floating Point Exception",
	"Virtualization Exception",
};


void IdtSetGate(uint8_t num, uint64_t offset, uint16_t selector, uint8_t flags)
{
	IDT64[num].offsetLow = offset & 0xFFFF;
	IDT64[num].offsetMiddle = (offset & 0xFFFF0000)>>16;
	IDT64[num].offsetHigh = (offset & 0xFFFFFFFF00000000)>>32;
	IDT64[num].codeSelector = selector;
	IDT64[num].flags = flags;

	IDT64[num].zero = 0;
	IDT64[num].reserved = 0;
}


void IdtLoad()
{
	IDT64_info.length = (sizeof(struct idt_gate)*256)-1;
	IDT64_info.base = &IDT64;

	for (int i=0; i < 21; i++) {
		IdtSetGate(i, (uint64_t)EXCEPTIONS[i], 0x08, 0x8E);
	}
	IdtSetGate(0xFF, isrFF, 0x08, 0x8E);

	LIDT(&IDT64_info);
}


void fault_handler(struct interrupt_frame *frame)
{
	char *buf[17];

	if (frame->number < 32) {
		Println("");
		Println("!!! SYSTEM HALT !!!");
		Println("");
		Println(EXCEPTION_NAMES[frame->number]);
		Println("");

		Print("  number = 0x");
		Println(Hexstring(buf,16, frame->number));
		Print("   error = 0x");
		Println(Hexstring(buf,16, frame->error_code));
		Print("     RIP = 0x");
		Println(Hexstring(buf,16, frame->rip));
		Print("      CS = 0x");
		Println(Hexstring(buf,16, frame->cs));
		Print("  RFLAGS = 0x");
		Println(Hexstring(buf,16, frame->rflags));
		Print("     RSP = 0x");
		Println(Hexstring(buf,16, frame->rsp));
		Print("      SS = 0x");
		Println(Hexstring(buf,16, frame->ss));
		Print("     CR2 = 0x");
		Println(Hexstring(buf,16, frame->cr2));
		for (;;);
	} else {
		Print("!!! INTERRUPT vector=0x");
		Println(Hexstring(buf,16, frame->number));
	}
}
 