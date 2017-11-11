#include "pit.h"
#include "arch/x86.h"

static uint32_t pic_sleep_ticks_count = 0;
static uint8_t pic_sleep_ticks_locked = 0;

void PitSetFrequency(uint16_t frequency)
{
	OUTB(PORT_PIT_CHANNEL0, frequency & 0xFF);
	OUTB(PORT_PIT_CHANNEL0, frequency>>8 & 0xFF);
}



void PicEndOfInterrupt()
{
	OUTB(PORT_PIC_MASTER_CMD, PIC_CMD_RESET);
}


void PicRemap()
{
	OUTB(PORT_PIC_MASTER_CMD, 0x11);
	OUTB(PORT_PIC_SLAVE_CMD, 0x11);

	OUTB(PORT_PIC_MASTER_DATA, 0x20);
	OUTB(PORT_PIC_SLAVE_DATA, 0x28);

	OUTB(PORT_PIC_MASTER_DATA, 0x04);
	OUTB(PORT_PIC_SLAVE_DATA, 0x02);

	OUTB(PORT_PIC_MASTER_DATA, 0x01);
	OUTB(PORT_PIC_SLAVE_DATA, 0x01);

	OUTB(PORT_PIC_MASTER_DATA, 0x00);
	OUTB(PORT_PIC_SLAVE_DATA, 0x00);
}


void PicSleepTicks(int ticks)
{
	pic_sleep_ticks_locked = 1;
	pic_sleep_ticks_count = ticks;
	while (pic_sleep_ticks_locked) {
		//spinlock
	}
}

void PicHandleTick()
{
	pic_sleep_ticks_count -= 1;
	while (pic_sleep_ticks_count--) {
		//spindown
	}
	pic_sleep_ticks_locked = 0;
}