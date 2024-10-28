
#include "s21_strlen.h"

s21_size_t s21_strlen(const char *str) {
  if (str == S21_NULL) {
    return 0;
  }

  s21_size_t length = 0;
  while (*str != '\0') {
    ++length;
    ++str;
  }

  return length;
}
