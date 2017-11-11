#ifndef BAREBONES_PIT_H
#define BAREBONES_PIT_H
#include <inttypes.h>



#define PORT_PIT_CHANNEL0 0x40
#define PORT_PIT_MODE 0x43

void PitSetFrequency(uint16_t frequency);

#endif
