#include <Plain/Common.h>
#include <Plain/Kernel/Arch/x86_64/Arch.h>

struct GDT_Table {
  struct GDT_Entry null;
  struct GDT_Entry kern_code;
  struct GDT_Entry kern_data;
  struct GDT_Entry user_code;
  struct GDT_Entry user_data;
  struct GDT_Entry64 tss;
} _ATTRIBUTE(packed);

_ATTRIBUTE(aligned(4096))
struct GDT_Table gGDT_Table;

_ATTRIBUTE(aligned)
struct TSS gTSS;

static void BufferFillZeros(void *s, size_t n) {
    for(size_t i = 0; i < n; i++)
        ((uint8_t*)s)[i] = 0;
}

static void GDT_Entry_Initialize(struct GDT_Entry *this,
                                 uint32_t base, uint32_t lim,
                                 uint8_t acs, uint8_t flags)
{

}

static void GDT_Entry64_Initialize(struct GDT_Entry64 *this,
                                   uint64_t base, uint64_t lim,
                                   uint8_t acs, uint8_t flags)
{

}

static void SetupGDT()
{
    BufferFillZeros(&gTSS, sizeof(gTSS));
#define X 0xFFFFF

    GDT_Entry_Initialize(&gGDT_Table.null, 0, 0, 0, 0);
    GDT_Entry_Initialize(&gGDT_Table.kern_code, 0, X, 0x9A, 0xA);
    GDT_Entry_Initialize(&gGDT_Table.kern_data, 0, X, 0x92, 0xC);
    GDT_Entry_Initialize(&gGDT_Table.user_code, 0, X, 0xFA, 0xA);
    GDT_Entry_Initialize(&gGDT_Table.user_data, 0, X, 0xF2, 0xC);
    GDT_Entry64_Initialize(&gGDT_Table.tss, 0, X, 0x89, 0x0);
    
#undef X
    
    Arch_x86_64_LoadGDT(&gGDT_Table.null, sizeof(gGDT_Table));
}

void Arch_InitMemory()
{

}

void Arch_DisableInterrupts() { asm volatile("cli"); }
void Arch_EnableInterrupts() { asm volatile("sti"); }
void Arch_Halt() { for(;;) asm volatile("hlt"); }

/* https://forum.osdev.org/viewtopic.php?p=315435#p315435 Thanks! */
/* void Arch_Interrupt(uint8_t i) */
asm(".pushsection .text\n\t"

     /* Generate the table of interrupt calls */
     ".align 4\n"
     "int_jmp_table:\n\t"
     "intno=0\n\t"
     ".rept 256\n\t"
         "\tint $intno\n\t"
         "\tret\n\t"
         "\t.align 4\n\t"
         "\tintno=intno+1\n\t"
     ".endr\n\t"

     /* generate_interrupt function */
     ".global Arch_Interrupt\n"             /* Give this function global visibility */
     "Arch_Interrupt:\n\t"
     "movzx %dil, %edi\n\t"                 /* Zero extend int_no (in DIL) across RDI */
     "lea int_jmp_table(%rip), %rax\n\t"    /* Get base of interrupt jmp table */
     "lea (%rax,%rdi,4), %rax\n\t"          /* Add table base to offset = jmp address */
     "jmp *%rax\n\t"                        /* Do sepcified interrupt */
     ".popsection");

