#include <stdint.h>

extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	//print Main 
	long *video_mem	= (long *)0xb8000;
	video_mem[20] = 0x2f6e2f692f612f4d;

	video_mem[40] = multiboot2_magic;
}