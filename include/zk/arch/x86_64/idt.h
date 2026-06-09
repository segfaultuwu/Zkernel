#ifndef ZK_ARCH_X86_64_IDT_H
#define ZK_ARCH_X86_64_IDT_H

#include <stdint.h>

void idt_init(void);

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void exception_handler(uint64_t vector, uint64_t error_code);

#endif
