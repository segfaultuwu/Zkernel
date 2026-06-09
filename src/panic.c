#include <zk/panic.h>
#include <zk/graphics.h>

__attribute__((noreturn))
void panic(const char *msg) {
    print("\033[31m");
    println("");
    println("================================");
    println("          KERNEL PANIC");
    println("================================");
    print("\033[0m");

    if (msg != 0) {
        kprintf("Reason: %s\n", msg);
    } else {
        println("Reason: unknown");
    }

    println("");
    println("System halted.");

    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}
