#include <Plain/_Kernel/Common.h>
#include <Plain/_Kernel/Arch/x86_64/Arch.h>

struct GDT_Table {
    struct GDT_Entry null;  /* 0x00 */
    struct GDT_Entry kcode; /* 0x08 */
    struct GDT_Entry kdata; /* 0x10 */
    struct GDT_Entry udata; /* 0x18 */
    struct GDT_Entry ucode; /* 0x20 */
    struct GDT_Entry64 tss; /* 0x28 */
} __attribute__((packed));

// _Static_assert(sizeof(struct GDT_Table) == 0x40, "");

__attribute__((aligned(4096)))
struct GDT_Table gGDT_Table;

__attribute__((aligned))
struct TSS gTSS;

static void BufferFillZeros(void *s, size_t n) {
    for(size_t i = 0; i < n; i++)
        ((uint8_t*)s)[i] = 0;
}

static void GDT_Entry_Initialize(struct GDT_Entry *this,
                                 uint32_t base, uint32_t lim,
                                 uint8_t acs, uint8_t flags)
{
    this->lim0    = lim >> 00;
    this->lim1_fl = ((lim >> 16) & 0xF) | (flags << 4);

    this->base0 = (base >> 00) & 0xFF;
    this->base1 = (base >> 16) & 0xFF;
    this->base2 = (base >> 24) & 0xFF;

    this->type = acs;
}

static void GDT_Entry64_Initialize(struct GDT_Entry64 *this,
                                   uint64_t base, uint32_t lim,
                                   uint8_t acs, uint8_t flags)
{
    GDT_Entry_Initialize(&this->e, base, lim, acs, flags);
    this->base3 = base >> 32;
    this->res = 0;
}

#include <Plain/IO/Pipe.h>

static struct GDT_Pointer gGDT_Pointer;

static void SetupGDT()
{
    Pipe p = GetPipe(0);
    PutStr(p, "Setting up the GDT\n");

    BufferFillZeros(&gTSS, sizeof(gTSS) - 1);
    // gTSS.rsp0 = 0x0000000000000000; /* stack */
    // gTSS.iopb = sizeof(gTSS);

    PutStr(p, "* NULL Segment\n");
    GDT_Entry_Initialize(&gGDT_Table.null, 0, 0, 0, 0);
    PutStr(p, "* Kernel Code Segment\n");
    GDT_Entry_Initialize(&gGDT_Table.kcode, 0, 0xFFFFF, 0x9A, 0xA);
    PutStr(p, "* Kernel Data Segment\n");
    GDT_Entry_Initialize(&gGDT_Table.kdata, 0, 0xFFFFF, 0x92, 0xC);
    //PutStr(p, "* User NULL Segment\n");
    //GDT_Entry_Initialize(&gGDT_Table.unull, 0, 0, 0, 0);
    PutStr(p, "* User Code Segment\n");
    GDT_Entry_Initialize(&gGDT_Table.ucode, 0, 0xFFFFF, 0xFA, 0xA);
    PutStr(p, "* User Data Segment\n");
    GDT_Entry_Initialize(&gGDT_Table.udata, 0, 0xFFFFF, 0xF2, 0xC);
    PutStr(p, "* TSS Segment\n");
    GDT_Entry64_Initialize(&gGDT_Table.tss,
                           (uint64_t)&gTSS,
                           sizeof(gTSS) - 1,
                           0x89, 0x0);
#undef X

    PutStr(p, "Loading the GDT...\n");
    gGDT_Pointer.addr = (uint64_t)&gGDT_Table;
    gGDT_Pointer.limit = sizeof(gGDT_Table) - 1;
    Arch_x86_64_LoadGDT(&gGDT_Pointer);
    PutStr(p, "Done!\n");
    PutStr(p, "Loading TSS!\n");
    Arch_x86_64_LoadTSS(0x28);
    PutStr(p, "Done!\n");
}

void Arch_InitMemory()
{
    SetupGDT();
    /* Paging already done by BOOTBOOT? */
}

void Arch_DisableInterrupts() { asm volatile("cli"); }
void Arch_EnableInterrupts() { asm volatile("sti"); }
void Arch_Halt() { for(;;) asm volatile("hlt"); }

/* https://forum.osdev.org/viewtopic.php?p=315435#p315435 Thanks! */
/* void Arch_Interrupt(uint8_t i) */
// asm(".pushsection .text\n\t"
//
//      /* Generate the table of interrupt calls */
//      ".align 4\n"
//      "int_jmp_table:\n\t"
//      "intno=0\n\t"
//      ".rept 256\n\t"
//          "\tint $intno\n\t"
//          "\tret\n\t"
//          "\t.align 4\n\t"
//          "\tintno=intno+1\n\t"
//      ".endr\n\t"
//
//      /* generate_interrupt function */
//      ".global Arch_Interrupt\n"             /* Give this function global visibility */
//      "Arch_Interrupt:\n\t"
//      "movzx %dil, %edi\n\t"                 /* Zero extend int_no (in DIL) across RDI */
//      "lea int_jmp_table(%rip), %rax\n\t"    /* Get base of interrupt jmp table */
//      "lea (%rax,%rdi,4), %rax\n\t"          /* Add table base to offset = jmp address */
//      "jmp *%rax\n\t"                        /* Do sepcified interrupt */
//      ".popsection");
