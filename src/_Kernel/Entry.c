#include <Plain/Common.h>
#include <Plain/IO/Pipe.h>
#include <Plain/IO/FBuf/FBuf.h>
#include <Plain/IO/TTY/TTY.h>
#include <Plain/_Kernel/IO/Serial.h>
#include <Plain/_Kernel/Arch/Common.h>
#include <bootboot/bootboot.h>

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
Pipe GetPipe(uint32_t id)
{ return &gPipes[id]; }

static struct Framebuffer *gDisplay_Fb;
static struct BM_Font gBasicFont;

static struct Framebuffer gFBs[8];

/* Initializes system display */
void InitDisplay()
{
    extern BOOTBOOT ptr_bb;
    extern uint32_t ptr_fb;

    /* load basic font */
    extern char font8x8_basic[128][8];
    gBasicFont = (struct BM_Font){
        { "Font8x8 Basic", 8, 8 },
        &font8x8_basic
    };

    /* initialize the framebuffer */
    Framebuffer_Intiailize(gDisplay_Fb,
                           &ptr_fb,
                           ptr_bb.fb_width, ptr_bb.fb_height,
                           &gBasicFont.super);

}

static void InitDynLinker()
{

}

void _start()
{
    Arch_DisableInterrupts();
    Arch_InitSerial();

    InitPipes();
    Provider_Subscribe(&GetPipe(0)->prov, &gSerial);

    InitDisplay();

    Pipe p = GetPipe(0);
    PutStr(p, "Initializing Memory...\n");
    Arch_InitMemory();
    PutStr(p, "\033[0;32mDone!\n\033[m");

    PutStr(p, "Initializing Interrupts...\n");
    Arch_InitInterrupts();
    PutStr(p, "\033[0;32mDone!\n\033[m");

    // PutStr(p, "Testing interrupts... (0, 1, 2, 3)\n");
    // Arch_Interrupt(0);
    // Arch_Interrupt(1);
    // Arch_Interrupt(2);
    // Arch_Interrupt(3);
    // PutStr(p, "Done\n");

    PutStr(p, "Initializing Userland...\n");
    PutStr(p, "- \033[0;34mSyscalls...\033[m\n");
    Arch_InitSyscalls();
    PutStr(p, "\033[0;32m  Done!\n\033[m");
    PutStr(p, "- \033[0;34mDynamic Linker...\033[m\n");
    InitDynLinker();
    PutStr(p, "\033[0;32m  Done!\n\033[m");
    PutStr(p, "\033[0;32mDone!\n\033[m");

    PutStr(p, "\033[0;33mJumping to userland...\033[m\n");
    extern void TestUserlandFunction();
    Arch_GotoUserland(&TestUserlandFunction);

    PutStr(p, "\033[0;33mKernel Terminated\033[m\n");

    Arch_DisableInterrupts();
    Arch_Halt();
}

void TestUserlandFunction()
{
    asm("wrmsr");
    for(;;);
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
