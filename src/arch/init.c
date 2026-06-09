#include <zk/graphics.h>

#ifdef ZK_ARCH_X86_64
#include <zk/arch/x86_64/cpu.h>
#endif

void arch_init(void) {
#ifdef ZK_ARCH_X86_64
    log(LOG_INFO, "x86_64 Architecture Detected");
    cpu_init();
#else
    println("Unknown architecture");
#endif

    println("CPU Initialized");
}
