#include "s21_strcspn.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  if (str1 == S21_NULL || str2 == S21_NULL) {
    return 0;
  }

  s21_size_t count = 0;
  while (str1[count] != '\0') {
    s21_size_t str2Index = 0;
    while (str2[str2Index] != '\0') {
      if (str1[count] == str2[str2Index]) {
        return count;
      }
      ++str2Index;
    }
    ++count;
  }

  return count;
}