//
// Created by AdisAlagic on 19.11.2023.
//

#include "s21_strncpy.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *cDest = dest;
  const char *cSrc = src;
  s21_size_t i;

  for (i = 0; i < n && *cSrc != '\0'; i++) {
    *cDest++ = *cSrc++;
  }

  while (i++ < n) {
    *cDest++ = '\0';
  }

  return dest;
}
