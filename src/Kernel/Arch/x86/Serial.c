#include <Plain/Kernel/Serial.h>

#define COM1 0x3f8
#define PORT COM1

struct Subscriber gSerial = {0};

void OnGet_(void *serial, void *provider, uintptr_t data)
{
    if(data < ' ' && data != '\n' && data != '\r' && data != '\t') return;
    Serial_Write(data);
}

int Arch_InitSerial()
{
    gSerial.onget = (Subscriber_OnGet)&OnGet_;
    gSerial.this = &gSerial;
    return 0;
    outb(PORT + 1, 0x00); /* disable all interrupts */
    outb(PORT + 3, 0x80); /* enable DLAB (set baud rate divisor) */
    outb(PORT + 0, 0x03); /* set divisor to 3 (lo byte) 38400 baud */
    outb(PORT + 1, 0x00); /*                  (hi byte) */
    outb(PORT + 3, 0x03); /* 8 bits, no parity, one stop bit */
    outb(PORT + 2, 0xC7); /* enable FIFO, clear them, with 14-byte threshold */
    outb(PORT + 4, 0x0B); /* IRQs enabled, RTS/DSR set */
    outb(PORT + 4, 0x1E); /* set in loopback mode, test the serial chip */
    outb(PORT + 0, 0xAE); /* test serial chip (send byte 0xAE and check if serial returns same byte) */

    // check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT + 0) != 0xAE) {
        return 1;
    }

    // if serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);
    return 0;
}

static int SerialReceived()
{ return inb(PORT + 5) & 1; }

int IsTransmEmpty()
{ return inb(PORT + 5) & 0x20; }

void Serial_Write(uint8_t x)
{
    while(!IsTransmEmpty());
    outb(PORT, x);
}

uint8_t Serial_Read()
{
    while(!SerialReceived());
    return inb(PORT);
}
