#include "Plain/Common.h"
#include <Plain/_Kernel/Arch/Common.h>
#include <Plain/_Kernel/Arch/x86_64/Arch.h>
#include <Plain/_Kernel/Syscalls.h>

void Arch_x86_64_SyscallEntry() {}

static uint64_t GetMSR(uint32_t msr)
{
    uint32_t lo, hi;
    asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
    return ((uint64_t)hi << 32) | (uint64_t)lo;
}
 
static void SetMSR(uint32_t msr, uint64_t v)
{
    uint32_t lo = v >> 00,
             hi = v >> 32;
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

#define EFER_MSR 0xC0000080
#define STAR_MSR 0xC0000081
#define LSTAR_MSR 0xC0000082
#define SFMASK_MSR 0xC0000084

void Arch_InitSyscalls()
{
    Arch_x86_64_EnableSyscallExt();
    
    //SetMSR(EFER_MSR, GetMSR(EFER_MSR) | 1); // enable system call extensions
    //SetMSR(STAR_MSR, ((uint64_t)2 << 3 << 48) | ((uint64_t)1 << 3 << 32));
    //SetMSR(LSTAR_MSR, (uint64_t)&Arch_x86_64_SyscallEntry);
    //SetMSR(SFMASK_MSR, 0x47700); // TF|DF|IF|IOPL|AC|NT
}

#define USER_STACK_SIZE 1024
_ATTRIBUTE(aligned)
uint64_t gUserlandStack[USER_STACK_SIZE];

void Arch_GotoUserland(void *func)
{
    Arch_x86_64_Sysret((uint64_t)func,
                       (uint64_t)(&gUserlandStack[USER_STACK_SIZE - 1]));
}
