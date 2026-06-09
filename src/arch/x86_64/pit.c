#include <stdint.h>
#include <stddef.h>

#include <charlesnicholson/nanoprintf.h>

#include <zk/arch/x86_64/pit.h>
#include <zk/arch/x86_64/ports.h>

static volatile uint64_t g_pit_ticks = 0;
static uint32_t g_pit_frequency = 100;

void pit_init(uint32_t frequency) {
    if (frequency == 0) {
        frequency = 100;
    }

    g_pit_frequency = frequency;

    uint32_t divisor = PIT_FREQUENCY / frequency;

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xff));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xff));
}

void pit_handler(void) {
    g_pit_ticks++;
}

uint64_t pit_ticks(void) {
    return g_pit_ticks;
}

uint64_t pit_uptime_seconds(void) {
    return g_pit_ticks / g_pit_frequency;
}

uint64_t pit_uptime_milliseconds(void) {
    return (g_pit_ticks * 1000) / g_pit_frequency;
}

void pit_format_uptime(char *buf, uint64_t buf_size) {
    if (buf == NULL || buf_size == 0) {
        return;
    }

    uint64_t total_ms = pit_uptime_milliseconds();

    uint64_t ms = total_ms % 1000;
    uint64_t total_sec = total_ms / 1000;

    uint64_t sec = total_sec % 60;
    uint64_t total_min = total_sec / 60;

    uint64_t min = total_min % 60;
    uint64_t hours = total_min / 60;

    if (hours > 0) {
        npf_snprintf(
            buf,
            buf_size,
            "%luh %lumin %lu.%03lus",
            hours,
            min,
            sec,
            ms
        );
    } else if (min > 0) {
        npf_snprintf(
            buf,
            buf_size,
            "%lumin %lu.%03lus",
            min,
            sec,
            ms
        );
    } else {
        npf_snprintf(
            buf,
            buf_size,
            "%lu.%03lus",
            sec,
            ms
        );
    }

    buf[buf_size - 1] = '\0';
}

void pit_sleep(uint64_t ms) {
    uint64_t target = g_pit_ticks + ((ms * g_pit_frequency) / 1000);

    while (g_pit_ticks < target) {
        __asm__ volatile ("hlt");
    }
}
