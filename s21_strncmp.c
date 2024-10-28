//
// Created by Peanut Grodd on 11/17/23.
//

#include "s21_strncmp.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  s21_size_t i;
  int result = 0;

  for (i = 0; i < n && str1[i] != '\0' && str2[i] != '\0'; i++) {
    const unsigned char a = str1[i];
    const unsigned char b = str2[i];

    if (a != b) {
      result = a - b;
      break;
    }
  }

  if (i < n && result == 0) {
    result = str1[i] - str2[i];  // Длины строк несовпадают, возвращаем разницу
  }

  if (result > 0) result = 1;
  if (result < 0) result = -1;

  return result;
}
