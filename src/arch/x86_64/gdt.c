#include <zk/arch/x86_64/gdt.h>

#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

extern void gdt_flush(struct gdtr *gdtr);

static struct gdt_entry gdt[3];
static struct gdtr gdtr;

static struct gdt_entry gdt_make_entry(
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t flags
) {
    struct gdt_entry entry;

    entry.limit_low = limit & 0xffff;
    entry.base_low = base & 0xffff;
    entry.base_mid = (base >> 16) & 0xff;
    entry.access = access;
    entry.granularity = ((limit >> 16) & 0x0f) | (flags & 0xf0);
    entry.base_high = (base >> 24) & 0xff;

    return entry;
}

void gdt_init(void) {
    gdt[0] = gdt_make_entry(0, 0, 0, 0);

    /*
     * Kernel code:
     * access = 0x9A
     *   present = 1
     *   ring = 0
     *   code/data = 1
     *   executable = 1
     *   readable = 1
     *
     * flags = 0x20
     *   long mode = 1
     */
    gdt[1] = gdt_make_entry(0, 0, 0x9A, 0x20);

    /*
     * Kernel data:
     * access = 0x92
     *   present = 1
     *   ring = 0
     *   code/data = 1
     *   writable = 1
     *
     * flags = 0x00
     *   normal data segment in long mode
     */
    gdt[2] = gdt_make_entry(0, 0, 0x92, 0x00);

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint64_t)&gdt;

    gdt_flush(&gdtr);
}
