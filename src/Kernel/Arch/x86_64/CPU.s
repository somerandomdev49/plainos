
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
