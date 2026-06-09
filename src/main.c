#include "zk/arch/x86_64/gdt.h"
#include "zk/arch/x86_64/idt.h"
#include <zk/boot/limine.h>
#include <zk/graphics.h>

static void hcf(void) {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void kernel_main(void) {
    if (framebuffer_request.response == 0 ||
        framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    graphics_init();

    print("GDT Init...");
    gdt_init();
    println("Done");
    print("IDT Init...");
    idt_init();
    println("Done");

    println("\033[33mZKernel booted\033[0m");


    hcf();
}
