#include <Plain/Common.h>
#include <Plain/IO/Pipe.h>
#include <Plain/Kernel/Display/FBuf.h>
#include <Plain/Kernel/Display/Display.h>
#include <Plain/Kernel/Arch/Arch.h>
#include <Plain/Kernel/Serial.h>
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

/*-[ Kernel Initialization ]----------------------------------------------------*/

static struct Pipe gPipes[2];
static void InitPipes()
{
    Pipe_Initialize(&gPipes[0]);
    Pipe_Initialize(&gPipes[1]);
}

/* Return a global pipe */
Pipe GetTTY(uint32_t id)
{ return &gPipes[id]; }

static struct Framebuffer gDisplay_Fb;
static struct BM_Font gDisplay_Font;
static struct Display gDisplay;

/* Initializes system display */
void InitDisplay()
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

static void ConnectTTYs()
{
    Provider_Subscribe(&GetTTY(1)->prov, &gSerial);
    Provider_Subscribe(&GetTTY(0)->prov, &gDisplay.adp.sub);
}

void _start()
{
    Arch_DisableInterrupts();
    Arch_InitSerial();

    InitPipes();
    InitDisplay();
    ConnectTTYs();

    Pipe p = GetTTY(0);
    PutStr(p, "Initializing Memory... ");
    Arch_InitMemory();
    PutStr(p, "\033c2Done!\n\033c0");

    PutStr(p, "Initializing Interrupts... ");
    Arch_InitInterrupts();
    PutStr(p, "\033c2Done!\n\033c0");

    Arch_Interrupt(0);
    Arch_Interrupt(1);
    Arch_Interrupt(2);
    Arch_Interrupt(3);

    PutStr(p, "\033c2Kernel Terminated\033c0\n");

    Arch_DisableInterrupts();
    Arch_Halt();
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
        WriteFormat(GetPipe(/* id = */ 0), "%s\n", children[i]);
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
