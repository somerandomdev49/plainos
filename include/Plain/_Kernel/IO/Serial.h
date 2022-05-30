#ifndef PLAIN_KERNEL_SERIAL_
#define PLAIN_KERNEL_SERIAL_
#include <Plain/Common.h>

extern struct Subscriber gSerial;

void Serial_Write(uint8_t);
uint8_t Serial_Read();

#endif
