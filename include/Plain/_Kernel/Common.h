#ifndef PLAIN_KERNEL_COMMON_
#define PLAIN_KERNEL_COMMON_
#include <Plain/Common.h>

uint8_t clz(uint64_t x);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif
