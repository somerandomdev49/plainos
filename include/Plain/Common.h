#ifndef PLAIN_KERNEL_COMMON_
#define PLAIN_KERNEL_COMMON_

#include <stdint.h>
#include <stddef.h>

#define _ATTRIBUTE(...) __attribute__((__VA_ARGS__))

/* fwd */ struct Subscriber;
/* fwd */ struct Provider;

typedef /*- Function called by a provider -*/
void (*Subscriber_OnGet)(struct Subscriber *this,
                         struct Provider *provider,
                         void *data);

/*- Receives data from a provider -*/
struct Subscriber
{
    void *this;             /* pointer to this */
    Subscriber_OnGet onget; /* callback (uses `this`) */
};

/*- Sends data to a subscriber -*/
struct Provider
{
    uint32_t count;           /* amount of subs registered */
    struct Subscriber **subs; /* pointer to list of subs */
};

/* Adds `sub` to the subscriber list */
void Provider_Subscribe(struct Provider *this,
                        struct Subscriber *sub);

/* Provides `data` to all subscribers */
void Provider_Provide(struct Provider *this,
                      void *data);

static inline uint8_t clz(uint64_t x) {
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

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif
