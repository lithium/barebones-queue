#ifndef BAREBONES_PIT_H
#define BAREBONES_PIT_H
#include <inttypes.h>



#define PORT_PIC_MASTER_CMD 0x20
#define PORT_PIC_MASTER_DATA 0x21
#define PORT_PIC_SLAVE_CMD 0xA0
#define PORT_PIC_SLAVE_DATA 0xA1

#define PIC_CMD_RESET 0x20


#define PORT_PIT_CHANNEL0 0x40
#define PORT_PIT_MODE 0x43

void PitSetFrequency(uint16_t frequency);

void PicRemap(uint8_t offset);





void PicHandleTick();

void PicSleepTicks(int ticks);
#endif
