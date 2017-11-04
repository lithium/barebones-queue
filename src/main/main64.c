#include <stdint.h>
#include "vga/vga.h"

extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	char hellostring[10] = "hello 00\0\0";

	for (int i=0; i < 100; i++) {
		hellostring[6] = '0' + i / 10;
		hellostring[7] = '0' + i % 10;
		Println(hellostring, 7);
	}

}