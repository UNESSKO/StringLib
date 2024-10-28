#include "s21_strpbrk.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  if (str1 == S21_NULL || str2 == S21_NULL) {
    return S21_NULL;
  }
  /* GCC FAILED: ‘nonnull’ argument ‘str1’ compared to NULL
   * [-Werror=nonnull-compare] */

  while (*str1 != '\0') {
    const char *current = str2;
    while (*current != '\0') {
      if (*str1 == *current) {
        return (char *)str1;  // Найден соответствующий символ
      }
      ++current;
    }
    ++str1;
  }

  return S21_NULL;  // Нет соответствующего символа
}