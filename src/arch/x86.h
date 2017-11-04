#ifndef BAREBONES_ARCH_X86_H 
#define BAREBONES_ARCH_X86_H 
#include <inttypes.h>


inline uint8_t INB(uint16_t port) __attribute__((always_inline))
{
   uint8_t ret;
   asm volatile ("inb %%dx,%%al"
   			:"=a" (ret)
   			:"d" (port));
   return ret;
}

inline void OUTB(uint16_t port, uint8_t value) __attribute__((always_inline))
{
   asm volatile ("outb %%al,%%dx"
   			: 
   			:"d" (port), "a" (value));
}

#endif
