#include <Plain/Kernel/Exception.h>
#include <Plain/IO/Pipe.h>

const char *Kernel_ExceptionName[EXCEPTION_MAX_] =
{
    "Memory Access",
    "Divide By Zero",
    "Unknown"
};

/* Generate an exception */
void Kernel_Exception(enum Kernel_Exception ex,
                      const char *reason)
{
    Pipe p = GetTTY(0);
    Write(p, "\n\033c1Exception: \033c0");
    Write(p, Kernel_ExceptionName[ex]);
    if(reason != NULL)
    {
        Write(p, "\n   Reason: \033c2");
        Write(p, reason);
    }
    Write(p, "\033c0\n\n");
    
    // for(;;) asm("hlt");
}
