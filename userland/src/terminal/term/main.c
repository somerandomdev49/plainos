#include "Plain/Common.h"
#include <Plain/IO/FBuf/FBuf.h>
#include <Plain/IO/TTY/TTY.h>
#include <Plain/Sys.h>

// void OnGet(void *this, void *inp, )

void Main()
{
    struct Framebuffer *fb = SysGetFBuf(0);
    Pipe out = SysGetPipe(PIPE_STDOUT);
    Pipe inp = SysGetPipe(PIPE_STDINP);

    // struct Subscriber sub;
    // sub.onget = (Subscriber_OnGet)&OnGet;

    // Provider_Subscribe(inp, );
}
