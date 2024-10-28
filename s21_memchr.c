//
// Created by helge on 18.11.2023.
//

#include "s21_memchr.h"

#include "s21_strlen.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *res = S21_NULL;
  if (str != S21_NULL) {
    char *start = (char *)str;
    while (n != 0) {
      if (*start == c) {
        res = (void *)start;
        break;
      }
      n--;
      start++;
    }
  }
  return res;
}