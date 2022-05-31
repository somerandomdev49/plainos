
/*-[ GDT Loading ]--------------------------------------------------------------*/
gdtr:
    gdtr.size:   .word 0            /* size */
    gdtr.offset: .quad 0            /* offset */

.global Arch_x86_64_LoadGDT
Arch_x86_64_LoadGDT:                /* rdi: pointer, si: size */
    subw 1, %si                     /* GDTR requires size to be 1 less */
    movw %si, (gdtr.size)           /* put the size */
    movq %rdi, (gdtr.offset)        /* put the offset */
    lgdtq (gdtr)                    /* load the GDT */

.global Arch_x86_64_LoadTSS
Arch_x86_64_LoadTSS:                /* rdi: TSS segment */
    ltr %di                         /* load the TSS */
    ret

.global Arch_x86_64_ReloadSegments
Arch_x86_64_ReloadSegments:
    pushq $0x08                     /* push code segment selector */
    leaq .reload, %rax              /* load address of reload_CS -> rax */
    pushq %rax                      /* push ^ */
    lretq                           /* far return */
.reload:
    movw $0x10, %ax                 /* push data segment into registers */
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret
