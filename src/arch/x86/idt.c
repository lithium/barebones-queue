#include "arch/x86/idt.h"
#include "vga/vga.h"
#include "string/String.h"
#include "pit/pit.h"


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
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void isr48();

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
	isr21, 
	isr22, 
	isr23, 
	isr24, 
	isr25, 
	isr26, 
	isr27, 
	isr28, 
	isr29, 
	isr30, 
	isr31, 
	isr32, 
	isr33, 
	isr34, 
	isr35, 
	isr36, 
	isr37, 
	isr38, 
	isr39, 
	isr40, 
	isr41, 
	isr42, 
	isr43, 
	isr44, 
	isr45, 
	isr46, 
	isr47, 
	isr48 
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
	"RESERVED",
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

	Memset(IDT64, 0, sizeof(struct idt_gate)*256);
	for (int i=0; i <= 48; i++) {
		IdtSetGate(i, (uint64_t)EXCEPTIONS[i], 0x08, 0x8E);
	}
	// IdtSetGate(0xFF, isrFF, 0x08, 0x8E);

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
		if (frame->number == 32) {
			// PIT timer IRQ0
			// Println("irq0");
			PicHandleTick();
		}
		else {
			Print("!!! INTERRUPT vector=0x");
			Println(Hexstring(buf,16, frame->number& 0xFF));
		}

		// End Of Interrupt
		OUTB(PORT_PIC_MASTER_CMD, PIC_CMD_RESET);
	}
}
 