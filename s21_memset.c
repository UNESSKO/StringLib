//
// Created by AdisAlagic on 18.11.2023.
//

#include "s21_memset.h"
void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *cpy = str;
  for (s21_size_t i = 0; i < n; i++) {
    cpy[i] = c;
  }
  return cpy;
}
