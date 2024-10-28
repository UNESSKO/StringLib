//
// Created by AdisAlagic on 18.11.2023.
//
#include "common.h"

#include <stdlib.h>

char *create_string(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }

  // Определение длины строки
  int length = 0;
  while (str[length] != '\0') {
    length++;
  }

  // Выделение памяти для новой строки
  char *duplicate = (char *)malloc((length + 1) * sizeof(char));

  if (duplicate == S21_NULL) {
    // Ошибка выделения памяти
    return S21_NULL;
  }

  // Копирование символов из исходной строки в новую
  for (int i = 0; i <= length; i++) {
    duplicate[i] = str[i];
  }

  return duplicate;
}
