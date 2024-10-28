#include "s21_toupper.h"

char *s21_to_upper(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t length = 0;
  length = s21_strlen(str);

  // Новая строка
  char *result = create_string(str);

  // Символы из исходной строки в новую, с преобразованием
  for (s21_size_t i = 0; i < length; ++i) {
    // Преобразование используя арифметику
    result[i] = (char)(is_lower(str[i]) ? str[i] - 'a' + 'A' : str[i]);
  }

  // Закрыть строку
  result[length] = '\0';

  return result;
}

int is_lower(char ch) { return ch >= 'a' && ch <= 'z'; }
