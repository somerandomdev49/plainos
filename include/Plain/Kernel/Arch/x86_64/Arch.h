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

/* loads an IDT at ptr, with size `size` */
void Arch_x86_64_LoadIDT(struct IDT_Entry *ptr,
                         uint16_t size);

#endif
