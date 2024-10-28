#include "s21_memcmp.h"

int s21_memcmp(const void *s1, const void *s2, s21_size_t n) {
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;

  for (s21_size_t i = 0; i < n; ++i) {
    if (p1[i] != p2[i]) {
      return p1[i] - p2[i];
    }
  }

  return 0;
}