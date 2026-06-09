#ifndef ZK_ARCH_X86_64_PIC_H
#define ZK_ARCH_X86_64_PIC_H

#include <stdint.h>

void pic_remap(void);
void pic_send_eoi(uint8_t irq);
void pic_unmask(uint8_t irq);
void pic_mask(uint8_t irq);

#endif
