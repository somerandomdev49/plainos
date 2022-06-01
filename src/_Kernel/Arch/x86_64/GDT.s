
/*-[ GDT Loading ]--------------------------------------------------------------*/

.global Arch_x86_64_LoadTSS
Arch_x86_64_LoadTSS:                /* rdi: TSS segment */
    ltr %di                         /* load the TSS */
    ret

.global Arch_x86_64_LoadGDT
Arch_x86_64_LoadGDT:                /* rdi: pointer */
    lgdtq (%rdi)                    /* load the GDT */
    movw $0x10, %ax                 /* push data segment into registers */
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    popq %rdi                       /* pop return address */
    movq $0x08, %rax
    pushq %rax                      /* push code segment selector */
    pushq %rdi                      /* push return address */
    lretq                           /* far return */
