#ifndef PLAIN_KERNEL_EXCEPTION_
#define PLAIN_KERNEL_EXCEPTION_
#include <Plain/Common.h>

enum Kernel_Exception
{
    EXCEPTION_MEMORY_ACCESS,
    EXCEPTION_DIVIDE_BY_ZERO,
    EXCEPTION_UNKNOWN,
    EXCEPTION_MAX_
};

extern const char *Kernel_ExceptionName[EXCEPTION_MAX_];

/* Generate an exception */
void Kernel_Exception(enum Kernel_Exception ex,
                      const char *reason,
                      uint64_t addr,
                      uint64_t info);

#endif
