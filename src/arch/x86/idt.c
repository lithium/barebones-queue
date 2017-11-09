#include "arch/x86/idt.h"
#include "vga/vga.h"


struct idt_gate IDT64[256];
struct idt_info IDT64_info;

extern void isr0();


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


	IdtSetGate(0, (uint64_t)isr0, 0x08, 0x8E);

	LIDT(&IDT64_info);
}


void fault_handler(struct interrupt_frame *frame)
{
	char *buf[17];

	Println("");
	Println("!!! SYSTEM HALT !!!");
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
	for (;;);
}
 