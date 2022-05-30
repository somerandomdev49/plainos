#ifndef PLAIN_KERNEL_ARCH_
#define PLAIN_KERNEL_ARCH_
#include <Plain/Common.h>

void Arch_InitInterrupts();
void Arch_InitMemory();
void Arch_DisableInterrupts();
void Arch_EnableInterrupts();
void Arch_Halt();
void Arch_Interrupt(uint8_t i);
void Arch_InitSerial();
void Arch_InitSyscalls();
void Arch_GotoUserland(void *func);

#endif
