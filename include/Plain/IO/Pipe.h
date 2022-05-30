#ifndef PLAIN_PIPE_
#define PLAIN_PIPE_
#include <Plain/Common.h>

/*- Provider and subscriver of any text data -*/
struct Pipe
{
    struct Provider prov;
    struct Subscriber sub;
    struct Subscriber *prov_data_[8];
};

void Pipe_Initialize(struct Pipe *this);

/*- Pipe pointer shorthand -*/
typedef struct Pipe *Pipe;

void WriteChar(Pipe to, char c);
void Write(Pipe to, const char *str);
void WriteLine(Pipe to, const char *str);

static inline void WriteUInt64_B16(Pipe to, uint64_t v)
{
    if(v >= 16) WriteUInt64_B16(to, v / 16);
    WriteChar(to, "0123456789ABCDEF"[v % 16]);
}

static inline void WriteUInt64_B10(Pipe to, uint64_t v)
{
    if(v >= 10) WriteUInt64_B10(to, v / 10);
    WriteChar(to, '0' + (v % 10));
}

static inline void WriteUInt64_B16L(Pipe to, uint64_t v)
{
    char a[17] = { '0','0','0','0',
                   '0','0','0','0',
                   '0','0','0','0',
                   '0','0','0','0', '\0' };
    if(v == 0) Write(to, a);
    else
    {
        a[clz(v) / 4] = '\0';
        Write(to, a);
        WriteUInt64_B16(to, v);
    }
}

static inline void WriteUInt64(Pipe to, uint64_t v)
{ WriteUInt64_B16(to, v); }

Pipe GetGlobalPipe(uint32_t global_id);
Pipe GetLocalPipe(uint32_t local_id);

#endif
