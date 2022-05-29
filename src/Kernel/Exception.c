#include <Plain/Kernel/Exception.h>
#include <Plain/IO/Pipe.h>

const char *Kernel_ExceptionName[EXCEPTION_MAX_] =
{
    "Memory Access",
    "Divide By Zero"
};

/* Generate an exception */
void Kernel_Exception(enum Kernel_Exception ex,
                      const char *reason)
{
    Write(GetGlobalPipe(0), Kernel_ExceptionName[ex]);
    Write(GetGlobalPipe(0), " exception occured!\n");
    Write(GetGlobalPipe(0), "Reason: ");
    Write(GetGlobalPipe(0), reason);
    Write(GetGlobalPipe(0), "\n");
}
