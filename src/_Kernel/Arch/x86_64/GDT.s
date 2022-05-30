
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
    ret

#if 0
    movw 0x28, %ax                  /* TSS segment is 0x28 */
    ltr %ax                         /* load TSS */
    movw $0x10, %ax                 /* push data segment into registers */
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    popq %rdi                       /* pop the return address */
    movq $0x08, %rax                /* kernel code segment is 0x08 */
    pushq %rax                      /* push the kernel code segment */
    pushq %rdi                      /* push the return address again */
    lretq                           /* do a far ret, like a normal ret but
                                       pop an extra argument of the stack
                                       and load it into cs */
#endif

.global Arch_x86_64_ReloadSegments
Arch_x86_64_ReloadSegments:
    pushq $0x08                     /* push code segment selector */
    leaq .reload_CS(%rip), %rax     /* load address of reload_CS -> rax */
    pushq %rax                      /* push ^ */
    lretq                           /* far return */
.reload_CS:
    movw $0x10, %ax                 /* push data segment into registers */
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret
