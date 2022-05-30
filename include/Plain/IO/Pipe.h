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

void PutChar(Pipe to, char c);
void PutStr(Pipe to, const char *str);
void PutStrLn(Pipe to, const char *str);

void PutU64_16(Pipe to, uint64_t v);
void PutU64_10(Pipe to, uint64_t v);
void PutU64_16L(Pipe to, uint64_t v);

static inline void PutU64(Pipe to, uint64_t v)
{ PutU64_16L(to, v); }

Pipe GetTTY(uint32_t id);
Pipe GetPipe(uint32_t local_id);

#endif
