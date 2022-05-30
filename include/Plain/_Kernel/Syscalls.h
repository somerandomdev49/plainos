#ifndef PLAIN_KERNEL_SYSCALLS_
#define PLAIN_KERNEL_SYSCALLS_
#include <Plain/_Kernel/Common.h>

struct SyscallFrame
{
    uint64_t out,
             in0,
             in1,
             in2;
};

void Syscall_GetPipe(struct SyscallFrame *frame);

#endif
