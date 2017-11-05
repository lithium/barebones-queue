#include <stdint.h>
#include "vga/vga.h"

#include "multiboot/Multiboot2Info.h"

extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	// char hellostring[10] = "hello 00\0\0";
	// for (int i=0; i < 100; i++) {
	// 	hellostring[6] = '0' + i / 10;
	// 	hellostring[7] = '0' + i % 10;
	// 	Println(hellostring, 7);
	// }


	Multiboot2Info mb2info;
	Multiboot2Info_create(&mb2info, multiboot2_info_addr);

	Print("Bootloader name: ", 7);
	Println(mb2info.bootLoaderNameTag->string, 7);

	Print("Commandline: ", 7);
	Println(mb2info.commandLineTag->string, 7);
}