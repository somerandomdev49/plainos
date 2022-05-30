#include <Plain/_Kernel/Common.h>

uint8_t clz(uint64_t x)
{
    uint64_t y;
    uint8_t n = 64, c = 32;
    do
    {
        y = x >> c;
        if(y != 0)
        {
            n -= c;
            x = y;
        }
        c >>= 1;
    } while(c != 0);
    return n - x;
}
