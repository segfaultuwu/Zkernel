ENTRY(kernel_main)

SECTIONS
{
    . = 0xffffffff80000000;

    .text : ALIGN(4K) {
        *(.text*)
    }

    .rodata : ALIGN(4K) {
        *(.rodata*)
    }

    .data : ALIGN(4K) {
        *(.data*)
    }

    .limine_requests_start : ALIGN(4K) {
        KEEP(*(.limine_requests_start))
    }

    .limine_requests : ALIGN(4K) {
        KEEP(*(.limine_requests))
    }

    .limine_requests_end : ALIGN(4K) {
        KEEP(*(.limine_requests_end))
    }

    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss*)
    }
}
