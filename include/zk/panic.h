#ifndef ZK_PANIC_H
#define ZK_PANIC_H

#include <stdnoreturn.h>

__attribute__((noreturn))
void panic(const char *msg);
#endif
