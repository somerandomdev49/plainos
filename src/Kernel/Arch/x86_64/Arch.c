#include <Plain/Common.h>
#include <Plain/Kernel/Arch/x86_64/Arch.h>
#include <Plain/Kernel/Exception.h>

static struct Arch_x86_64_IntDesc gIDT[256] _ATTRIBUTE(aligned);

static void InitGate(struct Arch_x86_64_IntDesc *g,
                     void *isr, uint8_t ring)
{
    g->off1 = ((uint64_t)isr >>  0) & 0x0000FFFF;
    g->off2 = ((uint64_t)isr >> 16) & 0x0000FFFF;
    g->off3 = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    
    g->zero = 0;
    g->sel = 0x08;
    
    g->opts.ist = 0;
    g->opts.res = 0;
    g->opts.gate = 14; /* 0b1110 */
    g->opts.dpl = ring;
    g->opts.pres = 1;
}

struct IntFrame
{
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

const char *Arch_x86_64_ExcNames[32] = {
    "Divide By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Float",
    "-Coprocessor-Segment-Overrun-",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Float Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved", /* 22 */
    "Reserved", /* 23 */
    "Reserved", /* 24 */
    "Reserved", /* 25 */
    "Reserved", /* 26 */
    "Reserved", /* 27 */
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

#define ISRFunc(N) Arch_x86_64_ISR##N
#define ExternISR(N) extern void ISRFunc(N) ();

/*- extern ISR declarations, implemented in assembly -*/

ExternISR(0);
ExternISR(1);
ExternISR(2);
ExternISR(3);
ExternISR(4);
ExternISR(5);
ExternISR(6);
ExternISR(7);
ExternISR(8);
ExternISR(9);
ExternISR(10);
ExternISR(11);
ExternISR(12);
ExternISR(13);
ExternISR(14);
ExternISR(15);
ExternISR(16);
ExternISR(17);
ExternISR(18);
ExternISR(19);
ExternISR(10);
ExternISR(21);
ExternISR(22);
ExternISR(23);
ExternISR(24);
ExternISR(25);
ExternISR(26);
ExternISR(27);
ExternISR(28);
ExternISR(29);
ExternISR(30);
ExternISR(31);

static void PageFault_Handler(uint64_t isr,
                       struct Arch_x86_64_Regs *regs)
{
    (void)isr; (void)regs;
    Kernel_Exception(EXCEPTION_MEMORY_ACCESS,
                     "CPU issued a Page Fault");
}

static void GeneralProtFault_Handler(uint64_t isr,
                                     struct Arch_x86_64_Regs *regs)
{
    (void)isr; (void)regs;
    Kernel_Exception(EXCEPTION_MEMORY_ACCESS,
                     "CPU issued a General Protection Fault");
}

void Arch_InitInterrupts()
{
#define INIT_GATE(N) InitGate(&gIDT[N], &ISRFunc(N), 0)
    INIT_GATE(0);
    INIT_GATE(1);
    INIT_GATE(2);
    INIT_GATE(3);
    INIT_GATE(4);
    INIT_GATE(5);
    INIT_GATE(6);
    INIT_GATE(7);
    INIT_GATE(8);
    INIT_GATE(9);
    INIT_GATE(10);
    INIT_GATE(11);
    INIT_GATE(12);
    INIT_GATE(13);
    INIT_GATE(14);
    INIT_GATE(15);
    INIT_GATE(16);
    INIT_GATE(17);
    INIT_GATE(18);
    INIT_GATE(19);
    INIT_GATE(10);
    INIT_GATE(21);
    INIT_GATE(22);
    INIT_GATE(23);
    INIT_GATE(24);
    INIT_GATE(25);
    INIT_GATE(26);
    INIT_GATE(27);
    INIT_GATE(28);
    INIT_GATE(29);
    INIT_GATE(30);
    INIT_GATE(31);
    
    Arch_x86_64_LoadIDT(gIDT, sizeof(gIDT));
    
    Arch_x86_64_RegisterInterrupt(14, &PageFault_Handler);
    Arch_x86_64_RegisterInterrupt(13, &GeneralProtFault_Handler);
    
    asm("sti");
}

/* TODO: Implement the Provider/Subscriber thing here */
static Arch_x86_64_Interrupt_Handler handlers[256] = {0};

void Arch_x86_64_RegisterInterrupt(uint8_t n,
                                   Arch_x86_64_Interrupt_Handler f)
{
    handlers[n] = f;
}

#include <Plain/IO/Pipe.h>

void Arch_x86_64_ISR_Handler(struct Arch_x86_64_Regs *regs)
{
    WriteLine(GetGlobalPipe(0), "ISR!");
    uint64_t isr = *(uint64_t*)(regs + 1);
    if(handlers[isr] != NULL) handlers[isr](isr, regs);
}
