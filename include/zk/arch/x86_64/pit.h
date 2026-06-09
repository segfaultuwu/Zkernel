#ifndef  ZK_ARCH_X86_64_PIT_H
#define ZK_ARCH_X86_64_PIT_H

#include <stdint.h>

#define PIT_FREQUENCY 1193182

void pit_init(uint32_t frequency);
void pit_handler(void);

uint64_t pit_ticks(void);
uint64_t pit_uptime_seconds(void);
uint64_t pit_uptime_ms(void);

void pit_format_uptime(char *buf, uint64_t buf_size);
void pit_sleep(uint64_t ms);

#endif
