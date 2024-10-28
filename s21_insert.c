#include "s21_insert.h"

/* Возвращает новую строку,
 * в которой указанная строка (str) вставлена в указанную позицию (start_index)
 * в данной строке (src). В случае какой-либо ошибки следует вернуть значение
 * NULL */

char *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL) {
    return S21_NULL;
  }

  if (s21_strlen(src) < start_index) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);
  if (start_index > src_len) {
    start_index = src_len;  // Вставка в конец
  }

  s21_size_t new_len = src_len + str_len;

  char *result = (char *)malloc((new_len + 1) * sizeof(char));

  if (result == S21_NULL) {
    return S21_NULL;
  }

  // Копирование начальной части строки
  s21_strncpy(result, src, start_index);

  // Копирование вставляемой строки
  s21_strncpy(result + start_index, str, str_len);

  // Копирование оставшейся части строки
  s21_strncpy(result + start_index + str_len, src + start_index,
              src_len - start_index);

  // Установка завершающего нулевого символа
  result[new_len] = '\0';

  return result;
}
