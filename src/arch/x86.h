#ifndef BAREBONES_ARCH_X86_H 
#define BAREBONES_ARCH_X86_H 
#include <inttypes.h>


inline void STI() __attribute__((always_inline))
{
   asm ("sti");
}

inline void CLI() __attribute__((always_inline))
{
   asm ("cli");
}

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

inline void LIDT(uint64_t idtAddress) __attribute__((always_inline))
{
	asm volatile ("lidt %0" 
			:
			: "m"(idtAddress));
}


inline uint64_t RDMSR(uint32_t msr_reg)  __attribute__((always_inline))
{
   uint64_t value;
   asm volatile ("rdmsr" 
      : "=A" (value)
      : "c" (msr_reg)
   );
   return value;
}

inline void WRMSR(uint32_t msr_reg, uint64_t value)  __attribute__((always_inline))
{
   asm volatile ("wrmsr"
      :
      : "c" (msr_reg), "A" (value)
   );
}

#endif
