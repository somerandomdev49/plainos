#ifndef PLAIN_TTY_
#define PLAIN_TTY_
#include <Plain/Common.h>

/*- Provider and subscriver of any text data -*/
struct Pipe
{
    struct Provider prov;
    struct Subscriber sub;
    struct Subscriber *prov_data_[8];
};

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

/*- Pipe pointer shorthand -*/
typedef struct Pipe *Pipe;

void WriteChar(Pipe tty, char c);
void Write(Pipe tty, const char *str);
void WriteLine(Pipe tty, const char *str);

#endif
