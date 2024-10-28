//
// Created by Filch Lenwood on 11/20/23.
//

#include "s21_to_lower.h"

void *s21_to_lower(const char *str) {
  if (str == S21_NULL) return S21_NULL;

  s21_size_t len = s21_strlen(str);
  char *result = (char *)malloc((len + 1) * sizeof(char));

  for (s21_size_t i = 0; i < len; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z')
      result[i] = str[i] + 32;
    else
      result[i] = str[i];
  }
  result[len] = '\0';  // Установка терминатора нулевого символа в конце строки

  return result;
}