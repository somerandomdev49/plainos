#ifndef PLAIN_KERNEL_DISPLAY_
#define PLAIN_KERNEL_DISPLAY_
#include <Plain/Common.h>

/*- Holds a framebuffer -*/
struct Display
{
    uint32_t id;
    struct Framebuffer *fb;
};

/* Initializes a display */
void Display_Initialize(struct Display *this,
                        uint32_t id,
                        struct Framebuffer *fbuf);

struct Display *GetDisplay(uint32_t id);

#endif
