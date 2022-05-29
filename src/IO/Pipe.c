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
