#include <stddef.h>
#include <stdint.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
  uint8_t *d = dest;
  const uint8_t *s = src;

  for (size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }

  return dest;
}

void *memset(void *dest, int value, size_t n) {
  uint8_t *d = dest;

  for (size_t i = 0; i < n; i++) {
    d[i] = (uint8_t)value;
  }

  return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *d = dest;
  const uint8_t *s = src;

  if (d < s) {
    for (size_t i = 0; i < n; i++) {
      d[i] = s[i];
    }
  } else {
    for (size_t i = n; i > 0; i--) {
      d[i - 1] = s[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *a, const void *b, size_t n) {
  const uint8_t *x = a;
  const uint8_t *y = b;

  for (size_t i = 0; i < n; i++) {
    if (x[i] != y[i]) {
      return x[i] - y[i];
    }
  }

  return 0;
}
