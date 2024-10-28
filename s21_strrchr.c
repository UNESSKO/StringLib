#include "s21_strrchr.h"

// Находит первый символ в строке str1, который соответствует любому символу,
// указанному в str2.
char *s21_strrchr(const char *str, int c) {
  const char *result = S21_NULL;

  if (str != S21_NULL) {
    do {
      if (*str == (char)c) {
        result = str;
      }
    } while (*str++);
  }
  return (char *)result;
}