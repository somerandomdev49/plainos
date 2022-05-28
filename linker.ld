
ptr_mmio = 0xfffffffff8000000;
ptr_fb = 0xfffffffffc000000;
ptr_bb = 0xffffffffffe00000;
ptr_env = 0xffffffffffe01000;

PHDRS
{
  boot PT_LOAD; /* one single loadable segment */
}
SECTIONS
{
    . = 0xffffffffffe02000;
    .text : {
        KEEP(*(.text.boot)) *(.text .text.*) /* code */
        *(.rodata .rodata.*) /* data */
        *(.data .data.*)
    } :boot
    .bss (NOLOAD) : { /* bss */
        . = ALIGN(16);
        *(.bss .bss.*)
        *(COMMON)
    } :boot

    /DISCARD/ : { *(.eh_frame) *(.comment) }
}
