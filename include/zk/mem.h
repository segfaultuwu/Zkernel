#ifndef ZK_MEM_H
#define ZK_MEM_H

#include <stddef.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *dest, int value, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *a, const void *b, size_t n);

void *kalloc(size_t size);
void kfree(void *ptr, size_t size);

#endif
