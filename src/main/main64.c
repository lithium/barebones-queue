#include <stdint.h>
#include "vga/vga.h"

#include "string/String.h"
#include "multiboot/Multiboot2Info.h"


extern uint32_t multiboot2_magic;
extern uint32_t multiboot2_info_addr;


void main64() 
{
	char hexstring[17];
	hexstring[16] = 0;

	// for (int i=0xFFFFFFFFF<<12; i <= -1; i++) {
		// Println(Hexstring(hexstring,16, i));
	// }

	Multiboot2Info mb2info;
	Multiboot2Info_create(&mb2info, multiboot2_info_addr);

	Print("Bootloader name: ");
	Println(mb2info.bootLoaderNameTag->string);

	Print("Commandline: ");
	Println(mb2info.commandLineTag->string);

	if (mb2info.basicMeminfoTag) {
		Print("Basic Meminfo: lower=0x");
		Print(Hexstring(hexstring, 16, mb2info.basicMeminfoTag->mem_lower));
		Print(" upper=0x");
		Print(Hexstring(hexstring, 16, mb2info.basicMeminfoTag->mem_upper));
	}
}