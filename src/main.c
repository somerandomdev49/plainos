#include <Plain/Common.h>
#include <Plain/Kernel/FBuf.h>
#include <Plain/IO/TTY.h>
#include <bootboot.h>

void printnum(struct Framebuffer *fb, uint32_t x, uint32_t y, uint32_t c, int n) {
    if(n >= 10) printnum(fb, x + 8, y, c, n / 10);
    Framebuffer_Char(fb, x, y, (n % 10) + '0', c);
}

struct Subscriber
{
    void (*onget)(void *provider, void *data);
};

struct Provider
{
    uint32_t count;
    struct Subscriber **providers;
};

struct DisplayChar
{
    uint32_t x, y, col;
    int char;
};

struct Display
{
    struct Framebuffer *fb;
};

void Display_Initialize(struct Display *this,
                        struct Framebuffer *fbuf)
{
    this->sub.count = 0;
    this->sub.providers = this->providers;
    this->fb = fbuf;
    this->providers = {0};
}

static ;

void InitDisplay()
{
    extern BOOTBOOT ptr_bb;
    extern uint32_t ptr_fb;

    extern char font8x8_basic[128][8];
    gDisplay_Font = (struct BM_Font){ { "Font8x8 Basic", 8, 8 }, &font8x8_basic };
    
    Framebuffer_Intiailize(&gDisplay_Fb,
                           &ptr_fb,
                           ptr_bb.fb_width, ptr_bb.fb_height,
                           (struct Font*)&bmfont);
}

void Display_SubscribeTo(struct Display *this, void *provider)
{
    
}

struct struct TTY gTTYs[2];
void InitTTYs()
{
    TTY_Initialize(&gTTYs[0]);
    TTY_Initialize(&gTTYs[1]);
}

void _start()
{
    InitDisplay();
    InitTTYs();
    
    /* this will render the tty buffer as it gets updated */
    Display_SubscribeTo(&gTTYs[0]);
    
    WriteLine(GetTTY(0), "Hello!");

    for(;;) asm("hlt");
}











#if 0
/*- ls implementation -*/
#include <Plain/FS/Directory.h>
#include <Plain/FS/Path.h>
#include <Plain/IO/TTY.h>
int main(int argc, char *argv[])
{
    const char *path = NULL;
    if(argc == 1) path = ".";
    else path = argv[1];

    unsigned long length = GetRealPath(/* path = */ path, /* buffer = */ NULL);
    char buffer[length];
    GetRealPath(/* path = */ path, /* buffer = */ buffer);
    
    struct DirectoryInfo info;
    DirectoryInfo_Init(&info, /* path = */ buffer);
    const char **children = DirectoryInfo_GetChildren(&info);

    for(unsigned int i = 0; childen[i]; ++i)
        WriteFormat(GetLocalTTY(/* id = */ 0), "%s\n", children[i]);
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
