#include "Plain/Common.h"
#include <Plain/_Kernel/Arch/Common.h>
#include <Plain/_Kernel/Arch/x86_64/Arch.h>
#include <Plain/_Kernel/Syscalls.h>

void Arch_InitSyscalls()
{
    Arch_x86_64_EnableSyscallExt();
}

#define USER_STACK_SIZE 1024
_ATTRIBUTE(aligned)
uint64_t gUserlandStack[USER_STACK_SIZE];

void Arch_GotoUserland(void *func)
{
    Arch_x86_64_Sysret((uint64_t)func, (uint64_t)(&gUserlandStack[USER_STACK_SIZE - 1]));
}
