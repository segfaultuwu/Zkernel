#include <zk/alloc.h>

#include <stdint.h>
#include <stddef.h>

#define HEAP_SIZE (1024 * 1024)

static uint8_t heap[HEAP_SIZE];
static size_t heap_offset = 0;

void *kalloc(size_t size) {
    size = (size + 15) & ~15;

    if (heap_offset + size > HEAP_SIZE) {
        return NULL;
    }

    void *ptr = &heap[heap_offset];
    heap_offset += size;

    return ptr;
}

void kfree(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
}
