#ifndef PLAIN_TTY_
#define PLAIN_TTY_
#include <Plain/Common.h>

/*- Provider and subscriver of any text data -*/
struct TTY
{
    struct Provider prov;
    struct Subscriber sub;
    struct Subscriber *prov_data_[8];
};

/* Subscriber : Called on get */
static void TTY_OnGet_(struct TTY *this,
                       void *provider,
                       void *ch)
{
    Provider_Provide(&this->prov, ch);
}


void TTY_Initialize(struct TTY *this)
{
    this->prov.count = 0;
    this->prov.subs = this->prov_data_;
    this->sub.this = this;
    this->sub.onget = (Subscriber_OnGet)&TTY_OnGet_;
}

/*- TTY pointer shorthand -*/
typedef struct TTY *TTY;

void WriteChar(TTY tty, char c);
void Write(TTY tty, const char *str);
void WriteLine(TTY tty, const char *str);

#endif
