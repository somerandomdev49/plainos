#include <Plain/IO/Pipe.h>

/* Subscriber : Called on get */
static void Pipe_OnGet_(struct Pipe *this,
                        void *provider,
                        void *ch)
{
    Provider_Provide(&this->prov, ch);
}

void Pipe_Initialize(struct Pipe *this)
{
    this->prov.count = 0;
    this->prov.subs = this->prov_data_;
    this->sub.this = this;
    this->sub.onget = (Subscriber_OnGet)&Pipe_OnGet_;
}

void PutChar(Pipe pipe, char c)
{
    pipe->sub.onget(pipe->sub.this, NULL, (void*)(uintptr_t)c);
}

void PutStr(Pipe pipe, const char *str)
{
    while(*str) PutChar(pipe, *str++);
}

void PutStrLn(Pipe pipe, const char *str)
{
    PutStr(pipe, str);
    PutChar(pipe, '\n');
}

void PutU64_10(Pipe to, uint64_t v)
{
    if(v >= 10) PutU64_10(to, v / 10);
    PutChar(to, '0' + (v % 10));
}

void PutU64_16(Pipe to, uint64_t v)
{
    if(v >= 16) PutU64_16(to, v / 16);
    PutChar(to, "0123456789ABCDEF"[v % 16]);
}

void PutU64_16L(Pipe to, uint64_t v)
{
    char a[17] = { '0','0','0','0',
                   '0','0','0','0',
                   '0','0','0','0',
                   '0','0','0','0', '\0' };
    if(v == 0) PutStr(to, a);
    else
    {
        a[clz(v) / 4] = '\0';
        PutStr(to, a);
        PutU64_16(to, v);
    }
}
