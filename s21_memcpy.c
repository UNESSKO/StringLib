//
// Created by AdisAlagic on 19.11.2023.
//

#include "s21_memcpy.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *cDest = dest;
  unsigned char *cSrc = (unsigned char *)src;
  for (s21_size_t i = 0; i < n; i++) {
    cDest[i] = cSrc[i];
  }
  return dest;
}
