#include "pit.h"
#include "arch/x86.h"


void PitSetFrequency(uint16_t frequency)
{
	OUTB(PORT_PIT_CHANNEL0, frequency & 0xFF);
	OUTB(PORT_PIT_CHANNEL0, frequency>>8 & 0xFF);
}