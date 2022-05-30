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
    g->attrs.zero = 0;
    g->attrs.type = 0x8E; // Trap Gate

    // g->opts.ist = 0;
    // g->opts.res = 0;
    // g->opts.gate = 14; /* 0b1110 */
    // g->opts.dpl = ring;
    // g->opts.pres = 1;
}


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

static void PageFault_Handler(struct Arch_x86_64_Frame *frame)
{
    Kernel_Exception(EXCEPTION_MEMORY_ACCESS,
                     "CPU issued a Page Fault");
}

static void GeneralProtFault_Handler(struct Arch_x86_64_Frame *frame)
{
    Kernel_Exception(EXCEPTION_MEMORY_ACCESS,
                     "CPU issued a General Protection Fault");
}

static void DivByZero_Handler(struct Arch_x86_64_Frame *frame)
{
    Kernel_Exception(EXCEPTION_DIVIDE_BY_ZERO, NULL);
}

static void Generic_Handler(struct Arch_x86_64_Frame *frame)
{
    Kernel_Exception(EXCEPTION_UNKNOWN, Arch_x86_64_ExcNames[frame->no]);
}

void Arch_InitInterrupts()
{
#define INIT_GATE(N) extern void ISRFunc(N)(); \
                     InitGate(&gIDT[N], &ISRFunc(N), 0)
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

    Arch_x86_64_LoadIDT(gIDT, (uint16_t)sizeof(gIDT));

    Arch_x86_64_RegisterInterrupt(00, &DivByZero_Handler);
    Arch_x86_64_RegisterInterrupt(14, &PageFault_Handler);
    Arch_x86_64_RegisterInterrupt(13, &GeneralProtFault_Handler);
    
    /* remap the PIC */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    
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

void WriteFrame(Pipe p, struct Arch_x86_64_Frame *frame)
{
#define W WriteUInt64_B16L
#define C "\033c3"
#define Z "\033c0"
    Write(p, "RDI = "C); W(p, frame->rdi); Write(p, "\t"Z);
    Write(p, "RSI = "C); W(p, frame->rsi); Write(p, "\t"Z);
    Write(p, "RBP = "C); W(p, frame->rbp); Write(p, "\n"Z);

    Write(p, "RDX = "C); W(p, frame->rdx); Write(p, "\t"Z);
    Write(p, "R8  = "C); W(p, frame->rdi); Write(p, "\t"Z);
    Write(p, "R9  = "C); W(p, frame->rsi); Write(p, "\n"Z);
    
    Write(p, "R10 = "C); W(p, frame->rbp); Write(p, "\t"Z);
    Write(p, "R11 = "C); W(p, frame->rdx); Write(p, "\t"Z);
    Write(p, "R12 = "C); W(p, frame->rdi); Write(p, "\n"Z);
    
    Write(p, "R13 = "C); W(p, frame->rsi); Write(p, "\t"Z);
    Write(p, "R14 = "C); W(p, frame->rbp); Write(p, "\t"Z);
    Write(p, "R15 = "C); W(p, frame->rdx); Write(p, "\n"Z);

    Write(p, "RCX = "C); W(p, frame->rcx); Write(p, "\t"Z);
    Write(p, "RBX = "C); W(p, frame->rbx); Write(p, "\t"Z);
    Write(p, "RAX = "C); W(p, frame->rax); Write(p, "\n"Z);
    
    Write(p, "ERR = "C); W(p, frame->err); Write(p, "\t"Z);
    Write(p, "NO  = "C); W(p, frame->no ); Write(p, "\t"Z);
    Write(p, "RIP = "C); W(p, frame->rip); Write(p, "\n"Z);
    
    Write(p, "CS  = "C); W(p, frame->cs ); Write(p, "\t"Z);
    Write(p, "RFL = "C); W(p, frame->rfl); Write(p, "\t"Z);
    Write(p, "RSP = "C); W(p, frame->rsp); Write(p, "\n"Z);
    Write(p, "SS  = "C); W(p, frame->ss ); Write(p, "\n"Z);
}

void Arch_x86_64_ISR_Handler(struct Arch_x86_64_Frame *frame)
{
    Pipe p = GetTTY(1);
    Write(p, "Interrupt #");
    WriteUInt64_B10(p, frame->no);
    Write(p, ", Frame:\n");
    WriteFrame(p, frame);
    if(handlers[frame->no] != NULL) handlers[frame->no](frame);
    else Generic_Handler(frame);
}

void Arch_InitMemory()
{

}


