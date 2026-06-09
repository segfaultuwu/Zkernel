#include <zk/arch/x86_64/cpu.h>
#include <zk/arch/x86_64/gdt.h>
#include <zk/arch/x86_64/idt.h>
#include <zk/arch/x86_64/pic.h>
#include <zk/arch/x86_64/pit.h>
#include <zk/graphics.h>

void cpu_init(void) {
    log(LOG_INFO, "GDT Init...");
    gdt_init();
    println("Done");

    log(LOG_INFO, "IDT Init...");
    idt_init();
    println("Done");

    log(LOG_INFO, "PIC Init...");
    pic_remap();
    pic_unmask(0);   // IRQ0 = PIT
    pit_init(100);   // 100 Hz
    println("Done");
}
