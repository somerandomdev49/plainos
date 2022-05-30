#include <Plain/IO/TTY/TTY.h>
#include <Plain/IO/FBuf/FBuf.h>

// /* Subscriber : Called on get */
// static void FBufTerm_OnGet_(struct TTY *this,
//                             void *provider,
//                             struct TTYCmd *cmd)
// {
//     (void)provider;
//     Framebuffer_Char(this->fb,
//                      c->x * (this->fb->font->width + 1),
//                      c->y * (this->fb->font->height + 1),
//                      c->ch, /* TODO: UTF-8 Framebuffer_Char */
//                      c->col);
// }

// /* Initializes a display */
// void Display_Initialize(struct Display *this,
//                         struct Framebuffer *fbuf)
// {
//     this->sub.this = this;
//     this->sub.onget = (Subscriber_OnGet)&Display_OnGet_;
//     this->fb = fbuf;
//     Adapter_DisplayChar_Initialize(&this->adp, &this->sub);
// }
