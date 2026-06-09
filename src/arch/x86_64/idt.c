#include "zk/arch/x86_64/pic.h"
#include "zk/arch/x86_64/pit.h"
#include <zk/arch/x86_64/idt.h>
#include <zk/graphics.h>

#include <stdint.h>
#include <stddef.h>

#define IDT_ENTRIES 256
#define IDT_INTERRUPT_GATE 0x8E

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

extern void idt_load(struct idtr *idtr);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);

static struct idt_entry idt[IDT_ENTRIES];
static struct idtr idtr;

static const char *exception_names[] = {
    "Divide by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 floating-point exception",
    "Alignment check",
    "Machine check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Control protection exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor injection exception",
    "VMM communication exception",
    "Security exception",
    "Reserved",
};

static void idt_set_gate(uint8_t vector, void (*handler)(void)) {
    uint64_t addr = (uint64_t)handler;

    idt[vector].offset_low = addr & 0xffff;
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].type_attr = IDT_INTERRUPT_GATE;
    idt[vector].offset_mid = (addr >> 16) & 0xffff;
    idt[vector].offset_high = (addr >> 32) & 0xffffffff;
    idt[vector].zero = 0;
}

void exception_handler(uint64_t vector, uint64_t error_code) {
    println("");
    println("=== ZKernel Exception ===");

    if (vector < 32) {
        kprintf("Vector: %lu\n", vector);
        kprintf("Name: %s\n", exception_names[vector]);
        kprintf("Error code: 0x%lx\n", error_code);
    } else {
        kprintf("Vector: %lu\n", vector);
        kprintf("Error code: 0x%lx\n", error_code);
    }

    println("Kernel halted.");

    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void idt_init(void) {
    idt_set_gate(0, isr0);
    idt_set_gate(1, isr1);
    idt_set_gate(2, isr2);
    idt_set_gate(3, isr3);
    idt_set_gate(4, isr4);
    idt_set_gate(5, isr5);
    idt_set_gate(6, isr6);
    idt_set_gate(7, isr7);
    idt_set_gate(8, isr8);
    idt_set_gate(9, isr9);
    idt_set_gate(10, isr10);
    idt_set_gate(11, isr11);
    idt_set_gate(12, isr12);
    idt_set_gate(13, isr13);
    idt_set_gate(14, isr14);
    idt_set_gate(15, isr15);
    idt_set_gate(16, isr16);
    idt_set_gate(17, isr17);
    idt_set_gate(18, isr18);
    idt_set_gate(19, isr19);
    idt_set_gate(20, isr20);
    idt_set_gate(21, isr21);
    idt_set_gate(22, isr22);
    idt_set_gate(23, isr23);
    idt_set_gate(24, isr24);
    idt_set_gate(25, isr25);
    idt_set_gate(26, isr26);
    idt_set_gate(27, isr27);
    idt_set_gate(28, isr28);
    idt_set_gate(29, isr29);
    idt_set_gate(30, isr30);
    idt_set_gate(31, isr31);
    idt_set_gate(32, isr32);

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t)&idt;

    idt_load(&idtr);
}

void irq_handler(uint64_t vector) {
    if (vector == 32) {
        pit_handler();
        pic_send_eoi(0);
        return;
    }

    if (vector >= 32 && vector <= 47) {
        pic_send_eoi(vector - 32);
    }
}
