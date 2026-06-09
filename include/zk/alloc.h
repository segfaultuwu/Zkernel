#ifndef ZK_ALLOC_H
#define ZK_ALLOC_H

#include <stddef.h>

void *kalloc(size_t size);
void kfree(void *ptr, size_t size);

#endif
