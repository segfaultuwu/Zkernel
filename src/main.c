#include <zk/arch/init.h>
#include <zk/arch/x86_64/pit.h>
#include <zk/boot/limine.h>
#include <zk/graphics.h>

static void hcf(void) {
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void kernel_main(void) {
    if (framebuffer_request.response == 0 ||
        framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    graphics_init();

    arch_init();

    println("\033[33mZKernel booted\033[0m");

    __asm__ volatile ("sti");

    uint64_t last = 0;

    for (;;) {
        uint64_t now = pit_ticks();

        if (now != last && now % 100 == 0) {
            kprintf("uptime: %lu sec\n", pit_uptime_seconds());
            last = now;
        }

        __asm__ volatile ("hlt");
    }
}
