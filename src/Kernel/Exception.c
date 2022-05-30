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
    Write(GetGlobalPipe(0), "\n\033c1Exception: \033c0");
    Write(GetGlobalPipe(0), Kernel_ExceptionName[ex]);
    if(reason != NULL)
    {
        Write(GetGlobalPipe(0), "\n   Reason: \033c2");
        Write(GetGlobalPipe(0), reason);
    }
    Write(GetGlobalPipe(0), "\033c0\n\n");
    
    for(;;) asm("hlt");
}
