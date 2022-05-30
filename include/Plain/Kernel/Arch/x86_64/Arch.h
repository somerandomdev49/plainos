#ifndef PLAIN_KERNEL_ARCH_X86_64_
#define PLAIN_KERNEL_ARCH_X86_64_
#include <Plain/Common.h>

/*- Loaded into the IDT -*/
struct IDT_Entry
{
   uint16_t off1; /* offset bits 0..15 */
   uint16_t sel;  /* a code segment selector in GDT or LDT */
   union {
       struct {
           uint16_t ist  : 3; /* IST, should be 0s (else for hardware tasks) */
           uint16_t res  : 5; /* reserved, assume 0s */
           uint16_t gate : 4; /* gate type, 0b1110 - interrupt, 0b1111 - trap */
           uint16_t zero : 1; /* 0 */
           uint16_t dpl  : 2; /* ring that is allowed to access the interrupt */
           uint16_t pres : 1; /* present bit, 1 */
       } opts;
       struct {
           uint8_t zero;
           uint8_t type;
       } attrs;
   };
   uint16_t off2; /* offset bits 16..31 */
   uint32_t off3; /* offset bits 32..63 */
   uint32_t zero; /* reserved */
} _ATTRIBUTE(packed);

_Static_assert(sizeof(struct IDT_Entry) == 16, "");

struct GDT_Entry
{
    uint16_t lim0;   /* limit 0..15 */
    uint16_t base0;  /* base 0..15 */
    uint8_t base1;   /* base 16..23 */
    uint8_t type;    /* type */
    uint8_t lim1_fl; /* limit 16..19 + flags */
    uint8_t base2;   /* base 24..31 */
} _ATTRIBUTE(packed);

_Static_assert(sizeof(struct GDT_Entry) == 8, "");

struct GDT_Entry64
{
    uint16_t lim0;   /* limit 0..15 */
    uint16_t base0;  /* base 0..15 */
    uint8_t base1;   /* base 16..23 */
    uint8_t type;    /* type */
    uint8_t lim1_fl; /* limit 16..19 + flags */
    uint8_t base2;   /* base 24..31 */
    uint32_t base3;  /* base 32..64 */
    uint32_t res;    /* reserved */
} _ATTRIBUTE(packed);

_Static_assert(sizeof(struct GDT_Entry64) == 16, "");

struct TSS
{
    uint32_t res0; /* reserved */
    uint64_t rsp0, /* stack pointer for ring 3|2|1 -> 0 (NB: not checked) */
             rsp1, /* stack pointer for ring 3|2 -> 1 */ 
             rsp2; /* stack pointer for ring 3 -> 2 */
    uint64_t res1; /* reserved */
    uint64_t ist2, /* Interrupt Stack Table, not used */
             ist3, /* not used */
             ist4, /* not used */
             ist5, /* not used */
             ist6, /* not used */
             ist7; /* not used */
    uint64_t res2; /* reserved */
    uint16_t res3; /* reserved */
    uint16_t iopb; /* IO Map Base Address Field (IO perm bit map, not used) */
} _ATTRIBUTE(packed); 

_Static_assert(sizeof(struct TSS) == 96, "");

/*- Registers -*/
struct StackFrame
{
    uint64_t r15,
             r14,
             r13,
             r12,
             r11,
             r10,
             r9,
             r8,
             rdi,
             rsi,
             rbp,
             rdx,
             rcx,
             rbx,
             rax;
    uint64_t no, err;
    uint64_t rip, cs, rfl, rsp, ss;
};

/* loads an GDT at ptr, with size `size` */
void Arch_x86_64_LoadIDT(struct IDT_Entry *ptr,
                         uint16_t size);

/* loads an IDT at ptr, with size `size` */
void Arch_x86_64_LoadGDT(struct GDT_Entry *ptr,
                         uint16_t size);

#endif
