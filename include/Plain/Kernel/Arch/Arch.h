#ifndef PLAIN_KERNEL_ARCH_
#define PLAIN_KERNEL_ARCH_
#include <Plain/Common.h>

extern void Arch_InitInterrupts();
extern void Arch_InitMemory();
extern void Arch_DisableInterrupts();
extern void Arch_EnableInterrupts();
extern void Arch_Halt();
extern void Arch_Interrupt(uint8_t i);
extern void Arch_InitSerial();

#endif
