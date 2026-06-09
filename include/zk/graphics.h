#ifndef ZK_GRAPHICS_H
#define ZK_GRAPHICS_H

void graphics_init(void);

void print(const char *text);
void println(const char *text);
void kprintf(const char *restrict format, ...);

#endif
