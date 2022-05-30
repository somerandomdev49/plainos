
/*-[ IDT Loading ]--------------------------------------------------------------*/
idtr:
    idtr.size:   .word 0            /* size */
    idtr.offset: .quad 0            /* offset */

.global Arch_x86_64_LoadIDT
Arch_x86_64_LoadIDT:                /* rdi: pointer, si: size */
    subw 1, %si                     /* IDTR requires size to be 1 less */
    movw %si, (idtr.size)           /* put the size */
    movq %rdi, (idtr.offset)        /* put the offset */
    lidtq (idtr)                    /* load the IDT */
    retq

/*-[ Push/Pop ]-----------------------------------------------------------------*/

/* Push all 64-bit registers (no rXX for now) */
.macro pushallq
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rbp
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
.endm

/* Pop all 64-bit registers (no rXX for now) */
.macro popallq
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    pop %rdi
    pop %rsi
    pop %rbp
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
.endm

/*-[ Common ISR Code ]----------------------------------------------------------*/

.extern Arch_x86_64_ISR_Handler
isr_common:                     /* common ISR handler */
    pushallq                    /* push all registers */

    cld                         /* SysV ABI req ig? */
    
    /* call the C handler */
    movq %rsp, %rdi             /* first C argument: pointer to struct */
    callq Arch_x86_64_ISR_Handler

    popallq                     /* pop all saved registers */

    addq $8, %rsp              /* pop pushed error code and ISR number */
    iretq                       /* return from ISR */


/*-[ ISR Macros ]---------------------------------------------------------------*/

.macro isr_e i                  /* ISR with error code pushed by CPU */
.global Arch_x86_64_ISR\i
.align 4
Arch_x86_64_ISR\i:
    pushq $\i                   /* push ISR index */
    jmp isr_common              /* jump to common handler */
.endm


.macro isr_n i                  /* ISR without error code pushed by CPU */
.global Arch_x86_64_ISR\i
.align 4
Arch_x86_64_ISR\i:
    pushq $0                    /* push our own "error code" instead */
    pushq $\i                   /* push ISR index */
    jmp isr_common              /* jump to common handler */
.endm

/*-[ ISRs ]---------------------------------------------------------------------*/

isr_n 0
isr_n 1
isr_n 2
isr_n 3
isr_n 4
isr_n 5
isr_n 6
isr_n 7
isr_e 8
isr_n 9
isr_e 10
isr_e 11
isr_e 12
isr_e 13
isr_e 14
isr_n 15
isr_n 16
isr_e 17
isr_n 18
isr_n 19
isr_n 20
isr_e 21
isr_n 22
isr_n 23
isr_n 24
isr_n 25
isr_n 26
isr_n 27
isr_n 28
isr_e 29
isr_e 30
isr_n 31
