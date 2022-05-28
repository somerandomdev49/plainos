.intel_syntax noprefix

idtr dw 0 /* size */
     dq 0 /* offset */

.global Arch_x86_64_LoadIDT
Arch_x86_64_LoadIDT:        /* rdi: pointer, si: size */
    sub si, 1               /* IDTR requires size to be 1 less */
    mov [idtr + 0], si      /* put the size */
    mov [idtr + 2], rdi     /* put the offset */
    lidt [idtr]             /* load the IDT */
    ret

