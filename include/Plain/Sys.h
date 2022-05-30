#ifndef PLAIN_SYS_
#define PLAIN_SYS_
#include <Plain/Common.h>
#include <Plain/IO/FBuf/FBuf.h>
#include <Plain/IO/TTY/TTY.h>

#define PIPE_STDOUT 0
#define PIPE_STDERR 1
#define PIPE_STDINP 2

/* syscall | returns a framebuffer */
struct Framebuffer *SysGetFBuf(uint32_t id);

/* syscall | returns a pipe (process local ids are negative) */
Pipe SysGetPipe(int32_t id);

static inline TTY SysGetTTY(uint32_t id) { return SysGetPipe(id); }

#endif
