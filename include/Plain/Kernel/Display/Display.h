#ifndef PLAIN_KERNEL_DISPLAY_INC_
#define PLAIN_KERNEL_DISPLAY_INC_
#include <Plain/Common.h>

/*- Represents a single character to
    be displayed on screen -*/
struct DisplayChar
{
    uint32_t x, y; /* position */
    uint32_t col;  /* foreground color */
    int ch;        /* UTF-8 character */
};

/*- Adapter from plain chars to display chars -*/
struct Adapter_DisplayChar
{
    struct Subscriber sub;
    struct Subscriber *tgt;
    uint32_t x, y;  /* cursor position */
    uint32_t col;   /* color */
    int e;          /* is in 'escape mode' */
    struct          /* escape mode data */
    {
        char mode;      /* mode */
        uint32_t *pal;  /* palette */
        size_t pal_max; /* palette max*/
    } ed;
};

void Adapter_DisplayChar_Initialize(struct Adapter_DisplayChar *this,
                                    struct Subscriber *target);

/*- Interface between a framebuffer and
    the system IO -*/
struct Display
{
    struct Subscriber sub;
    uint32_t id;
    struct Framebuffer *fb;
    struct Adapter_DisplayChar adp;
};

/* Initializes a display */
void Display_Initialize(struct Display *this,
                        struct Framebuffer *fbuf);
						
#endif
