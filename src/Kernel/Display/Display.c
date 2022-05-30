#include <Plain/Kernel/Display/Display.h>
#include <Plain/Kernel/Display/FBuf.h>
#include <Plain/IO/Pipe.h>
#include <bootboot.h>

typedef struct Adapter_DisplayChar Adapter_DC;

/* Subscriber : Called on get */
static void Adapter_DisplayChar_OnGet_(Adapter_DC *this,
                                       void *provider,
                                       uintptr_t ch)
{
    if(this->e && !this->ed.mode)
    {
        this->ed.mode = ch;
        return;
    }
    
    if(this->e && this->ed.mode == 'c')
    {
        if(ch - '0' < this->ed.pal_max)
            this->col = this->ed.pal[ch - '0'];
        this->ed.mode = 0;
    }
    
    if(this->ed.mode != 0 || this->e)
    {
        if(this->ed.mode == 0) this->e = 0; /* exit esc mode */
        return;
    }

    struct DisplayChar dc = {
        this->x + 1, this->y + 1,
        this->col,
        (int)ch
    };

    switch((char)ch)
    {
    case '\033': this->e = 1; return; /* Don't print the symbol */
    case '\n': this->x = 0; this->y++; break;
    case '\r': this->x = 0; break;
    case '\v': this->y++; break;
    default:   this->x++; break;
    }

    this->tgt->onget(this->tgt->this, (void*)this, &dc);
}

static uint32_t Adapter_DC_PALETTE0[] = {
    0xEFEFEF,
    0xDC143C,
    0x8FBC8F,
    0x1E90FF,
};

void Adapter_DisplayChar_Initialize(Adapter_DC *this,
                                    struct Subscriber *target)
{
    this->sub.this = this;
    this->sub.onget = (Subscriber_OnGet)&Adapter_DisplayChar_OnGet_;
    this->tgt = target;
    this->x = 0;
    this->y = 0;
    this->col = 0xEFEFEF;
    this->e = 0;
    this->ed.mode = 0;
    this->ed.pal = Adapter_DC_PALETTE0;
    this->ed.pal_max = sizeof(Adapter_DC_PALETTE0) / sizeof(uint32_t);
}

/* Subscriber : Called on get */
static void Display_OnGet_(struct Display *this,
                           void *provider,
                           struct DisplayChar *c)
{
    (void)provider;
    Framebuffer_Char(this->fb,
                     c->x * (this->fb->font->width + 1),
                     c->y * (this->fb->font->height + 1),
                     c->ch, /* TODO: UTF-8 Framebuffer_Char */
                     c->col);
}

/* Initializes a display */
void Display_Initialize(struct Display *this,
                        struct Framebuffer *fbuf)
{
    this->sub.this = this;
    this->sub.onget = (Subscriber_OnGet)&Display_OnGet_;
    this->fb = fbuf;
    Adapter_DisplayChar_Initialize(&this->adp, &this->sub);
}
