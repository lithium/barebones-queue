#include "pit.h"
#include "arch/x86.h"
#include "vga/vga.h"

static uint64_t pic_sleep_ticks_count = 0;

void PitSetFrequency(uint16_t frequencyInHz)
{
	uint16_t f = 1193180 / frequencyInHz;
	OUTB(PORT_PIT_CHANNEL0, f & 0xFF);
	OUTB(PORT_PIT_CHANNEL0, f>>8 & 0xFF);
}

void PicEndOfInterrupt()
{
	OUTB(PORT_PIC_MASTER_CMD, PIC_CMD_RESET);
}


void PicRemap(uint8_t offset)
{
	uint8_t master_mask = INB(PORT_PIC_MASTER_DATA);
	uint8_t slave_mask = INB(PORT_PIC_SLAVE_DATA);

	OUTB(PORT_PIC_MASTER_CMD, 0x11);
	OUTB(PORT_PIC_SLAVE_CMD, 0x11);

	OUTB(PORT_PIC_MASTER_DATA, offset);
	OUTB(PORT_PIC_SLAVE_DATA, offset+8);

	OUTB(PORT_PIC_MASTER_DATA, 0x04);
	OUTB(PORT_PIC_SLAVE_DATA, 0x02);

	OUTB(PORT_PIC_MASTER_DATA, 0x01);
	OUTB(PORT_PIC_SLAVE_DATA, 0x01);

	OUTB(PORT_PIC_MASTER_DATA, master_mask);
	OUTB(PORT_PIC_SLAVE_DATA, slave_mask);
}


void PicSleepTicks(uint64_t ticks)
{
	pic_sleep_ticks_count = ticks;
	while (pic_sleep_ticks_count > 0) {
		//spin
	}
}

void PicHandleTick()
{
	if (pic_sleep_ticks_count > 0) {
		pic_sleep_ticks_count--;
	}
}