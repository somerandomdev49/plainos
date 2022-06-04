
.global outb
outb:               /* di - port, si - value */
    movw %si, %ax   /* si -> ax, so that al can be used */
    movw %di, %dx   /* di -> dx, because outb only allows dx */
    outb %al, %dx   /* output byte */
    ret

.global inb
inb:                /* di - port */
    mov %di, %dx    /* inb only allows dx */
    inb %dx, %al    /* input byte */
    ret

.global Arch_x86_64_EnableSyscallExt
Arch_x86_64_EnableSyscallExt:
    movq $0xC0000080, %rcx      /* EFER MSR */
    rdmsr                       /* read current EFER */
    orl $1, %eax                /* enable SCE bit */
    wrmsr                       /* write back new EFER */
    mov $0xC0000081, %rcx       /* STAR MSR */
    rdmsr                       /* read current STAR */
    mov $0x00100008, %edx       /* [48:64]+8=UDATA, [48:64]+16=UCODE,
                                   [32:48]=KCODE, [32:48]+8=UDATA */
    wrmsr                       /* write back new STAR */
    ret                         /* return back to C */

.global Arch_x86_64_Sysret
Arch_x86_64_Sysret:
    movq %rdi, %rcx
    movq %rsi, %rsp
    movq $0x202, %r11
    sysretq
