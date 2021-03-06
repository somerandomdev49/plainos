#include <Plain/_Kernel/Exception.h>
#include <Plain/_Kernel/Arch/Common.h>
#include <Plain/IO/Pipe.h>

const char *Kernel_ExceptionName[EXCEPTION_MAX_] =
{
    "Memory Access",
    "Divide By Zero",
    "CPU-Specific"
};

/* Generate an exception */
void Kernel_Exception(enum Kernel_Exception ex,
                      const char *reason,
                      uint64_t addr,
                      uint64_t info)
{
    Pipe p = GetPipe(0);
    PutStr(p, "\n\033[0;31mException: \033[0;32m");
    PutStr(p, Kernel_ExceptionName[ex]);
    PutStr(p, "\033[m\n");
    if(reason != NULL)
    {
        PutStr(p, "   \033[0;31mReason: \033[0;32m");
        PutStr(p, reason);
        PutStr(p, "\033[m\n");
    }
    if(addr != 0x0)
    {
        PutStr(p, "    \033[0;31mWhere: \033[0;32m0x");
        PutU64_16L(p, addr);
        PutStr(p, "\033[m\n");
    }
    if(info != 0x0)
    {
        PutStr(p, "     \033[0;31mCode: \033[0;32m0x");
        PutU64_16(p, info);
        PutStr(p, "\033[m\n");
    }
    PutStr(p, "\n");
    Arch_Halt();
    // for(;;) asm("hlt");
}
