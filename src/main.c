#include <Plain/Common.h>
#include <Plain/Kernel/FBuf.h>
#include <Plain/IO/Pipe.h>
#include <bootboot.h>

/*-[ Plain/Common.h ]-----------------------------------------------------------*/

void Provider_Subscribe(struct Provider *this,
                        struct Subscriber *sub)
{
    this->subs[this->count++] = sub;
}

void Provider_Provide(struct Provider *this,
                      void *data)
{
    for(uint32_t i = 0; i < this->count; ++i)
        this->subs[i]->onget(this->subs[i]->this, this, data);
}

/*-[ Plain/IO/Pipe.h ]-----------------------------------------------------------*/

void WriteChar(Pipe pipe, char c)
{
    pipe->sub.onget(pipe->sub.this, NULL, (void*)(uintptr_t)c);
}

void Write(Pipe pipe, const char *str)
{
    while(*str) WriteChar(pipe, *str++);
}

void WriteLine(Pipe pipe, const char *str)
{
    Write(pipe, str);
    WriteChar(pipe, '\n');
}

/*-[ Plain/Kernel/Display.h ]---------------------------------------------------*/

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
    uint32_t x, y;
    uint32_t col;
};

typedef struct Adapter_DisplayChar Adapter_DC;

/* Subscriber : Called on get */
static void Adapter_DisplayChar_OnGet_(Adapter_DC *this,
                                       void *provider,
                                       uintptr_t ch)
{
    struct DisplayChar dc = {
        this->x, this->y,
        this->col,
        (int)ch
    };
    
    switch((char)ch)
    {
    case '\n': this->x = 0; this->y++; break;
    case '\r': this->x = 0; break;
    case '\v': this->y++; break;
    default:   this->x++; break;
    }
    
    this->tgt->onget(this->tgt->this, (void*)this, &dc);
}

void Adapter_DisplayChar_Initialize(Adapter_DC *this,
                                    struct Subscriber *target)
{
    this->sub.this = this;
    this->sub.onget = (Subscriber_OnGet)&Adapter_DisplayChar_OnGet_;
    this->tgt = target;
    this->x = 1;
    this->y = 1;
    this->col = 0xFFFFFF;
}

/*- Interface between a framebuffer and
    the system IO -*/
struct Display
{
    struct Subscriber sub;
    uint32_t id;
    struct Framebuffer *fb;
    struct Adapter_DisplayChar adp;
};

/* Subscriber : Called on get */
static void Display_OnGet_(struct Display *this,
                           void *provider,
                           struct DisplayChar *c)
{
    (void)provider;
    Framebuffer_Char(this->fb,
                     c->x * this->fb->font->width,
                     c->y * this->fb->font->height,
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

/*-[ Kernel Initialization ]----------------------------------------------------*/

static struct Framebuffer gDisplay_Fb;
static struct BM_Font gDisplay_Font;
static struct Display gDisplay;

/* Initializes system display */
static void InitDisplay()
{
    extern BOOTBOOT ptr_bb;
    extern uint32_t ptr_fb;

    /* load basic font */
    extern char font8x8_basic[128][8];
    gDisplay_Font = (struct BM_Font){
        { "Font8x8 Basic", 8, 8 },
        &font8x8_basic
    };
    
    /* initialize the framebuffer */
    Framebuffer_Intiailize(&gDisplay_Fb,
                           &ptr_fb,
                           ptr_bb.fb_width, ptr_bb.fb_height,
                           (struct Font*)&gDisplay_Font);
    
    /* initialize the display | TODO: multiple displays */
    Display_Initialize(&gDisplay, &gDisplay_Fb);
}

static struct Pipe gPipes[2];
static void InitPipes()
{
    Pipe_Initialize(&gPipes[0]);
    Pipe_Initialize(&gPipes[1]);
}

/* Return a global pipe */
static inline Pipe GetPipe(uint32_t global_id)
{ return &gPipes[global_id]; }

extern void Arch_InitInterrupts();

void _start()
{
    InitDisplay();
    InitPipes();
    
    /* this will render the Pipe as it gets updated */
    Provider_Subscribe(&GetPipe(0)->prov, &gDisplay.adp.sub);
    
    Write(GetPipe(0), "Initializing Interrupts... ");
    Arch_InitInterrupts();
    Write(GetPipe(0), "Done!");

    for(;;) asm("hlt");
}











/*-[ Usermode App Example ]-----------------------------------------------------*/
#if 0

/*- ls implementation -*/
#include <Plain/FS/Directory.h>
#include <Plain/FS/Path.h>
#include <Plain/IO/Pipe.h>
#include <Plain/UI/Args.h>

void Main()
{
    const char *path = NULL;

    if(GetArgCount() == 1)
        path = ".";
    else
        path = GetArg(1);

    unsigned long length = GetRealPath(/* path = */ path, /* buffer = */ NULL);
    char buffer[length];
    GetRealPath(/* path = */ path, /* buffer = */ buffer);
    
    struct DirectoryInfo info;
    DirectoryInfo_Init(&info, /* path = */ buffer);
    const char **children = DirectoryInfo_GetChildren(&info);

    for(unsigned int i = 0; childen[i]; ++i)
        WriteFormat(GetLocalPipe(/* id = */ 0), "%s\n", children[i]);
}

// Example:
/*---------------------------------.

/users/me# tcc ls.c -o ls
/users/me# by root mv ls /bin/ls
/users/me# ls
ls.c
rm.c
mkdir.c
touch.c
clear.c
/user/me# doas root shutdown

`---------------------------------*/
#endif
