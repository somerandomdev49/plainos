#include <Plain/_Kernel/Common.h>
#include <Plain/_Kernel/Arch/x86_64/Arch.h>
#include <Plain/_Kernel/Exception.h>

static struct IDT_Entry gIDT[256] _ATTRIBUTE(aligned);

static void InitGate(struct IDT_Entry *g,
                     void *isr, uint8_t ring)
{
    g->off1 = ((uint64_t)isr >>  0) & 0x0000FFFF;
    g->off2 = ((uint64_t)isr >> 16) & 0x0000FFFF;
    g->off3 = ((uint64_t)isr >> 32) & 0xFFFFFFFF;

    g->zero = 0;
    g->sel = 0x08;
    g->attrs.zero = 0;
    g->attrs.type = 0x8E;
}


const char *ExcNames[32] = {
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

static void PageFault_Handler(struct StackFrame *frame)
{
    Kernel_Exception(EXCEPTION_MEMORY_ACCESS,
                     "CPU issued a Page Fault",
                     frame->rip);
}

static void GeneralProtFault_Handler(struct StackFrame *frame)
{
    Kernel_Exception(EXCEPTION_MEMORY_ACCESS,
                     "CPU issued a General Protection Fault",
                     frame->rip);
}

static void DivByZero_Handler(struct StackFrame *frame)
{
    Kernel_Exception(EXCEPTION_DIVIDE_BY_ZERO, NULL, frame->rip);
}

static void Generic_Handler(struct StackFrame *frame)
{
    Kernel_Exception(EXCEPTION_UNKNOWN, ExcNames[frame->no], frame->rip);
}

typedef /*- Function called when an ISR is ran -*/
void (*IntHandler)(struct StackFrame *frame);

/* TODO: Implement the Provider/Subscriber thing here */
static IntHandler handlers[256] = {0};

static void RegisterInterrupt(uint8_t n, IntHandler f)
{
    handlers[n] = f;
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

    RegisterInterrupt(00, &DivByZero_Handler);
    RegisterInterrupt(14, &PageFault_Handler);
    RegisterInterrupt(13, &GeneralProtFault_Handler);
    
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

#include <Plain/IO/Pipe.h>

static void WriteFrame(Pipe p, struct StackFrame *frame)
{
#define W PutU64_16L
#define C "\033[0;33m"
#define Z "\033[m"
    PutStr(p, "RDI = "C); W(p, frame->rdi); PutStr(p, "\t"Z);
    PutStr(p, "RSI = "C); W(p, frame->rsi); PutStr(p, "\t"Z);
    PutStr(p, "RBP = "C); W(p, frame->rbp); PutStr(p, "\n"Z);

    PutStr(p, "RDX = "C); W(p, frame->rdx); PutStr(p, "\t"Z);
    PutStr(p, "R8  = "C); W(p, frame->rdi); PutStr(p, "\t"Z);
    PutStr(p, "R9  = "C); W(p, frame->rsi); PutStr(p, "\n"Z);
    
    PutStr(p, "R10 = "C); W(p, frame->rbp); PutStr(p, "\t"Z);
    PutStr(p, "R11 = "C); W(p, frame->rdx); PutStr(p, "\t"Z);
    PutStr(p, "R12 = "C); W(p, frame->rdi); PutStr(p, "\n"Z);
    
    PutStr(p, "R13 = "C); W(p, frame->rsi); PutStr(p, "\t"Z);
    PutStr(p, "R14 = "C); W(p, frame->rbp); PutStr(p, "\t"Z);
    PutStr(p, "R15 = "C); W(p, frame->rdx); PutStr(p, "\n"Z);

    PutStr(p, "RCX = "C); W(p, frame->rcx); PutStr(p, "\t"Z);
    PutStr(p, "RBX = "C); W(p, frame->rbx); PutStr(p, "\t"Z);
    PutStr(p, "RAX = "C); W(p, frame->rax); PutStr(p, "\n"Z);
    
    PutStr(p, "ERR = "C); W(p, frame->err); PutStr(p, "\t"Z);
    PutStr(p, "NO  = "C); W(p, frame->no ); PutStr(p, "\t"Z);
    PutStr(p, "RIP = "C); W(p, frame->rip); PutStr(p, "\n"Z);
    
    PutStr(p, "CS  = "C); W(p, frame->cs ); PutStr(p, "\t"Z);
    PutStr(p, "RFL = "C); W(p, frame->rfl); PutStr(p, "\t"Z);
    PutStr(p, "RSP = "C); W(p, frame->rsp); PutStr(p, "\n"Z);
    PutStr(p, "SS  = "C); W(p, frame->ss ); PutStr(p, "\n"Z);
}

void Arch_x86_64_ISR_Handler(struct StackFrame *frame)
{
    Pipe p = GetPipe(0);
    PutStr(p, "Interrupt #");
    PutU64_10(p, frame->no);
    PutStr(p, ", Frame:\n");
    WriteFrame(p, frame);
	
    if(handlers[frame->no] != NULL) handlers[frame->no](frame);
    else Generic_Handler(frame);
}
