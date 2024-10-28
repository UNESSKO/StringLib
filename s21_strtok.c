#include "s21_strtok.h"

char *s21_strtok(char *str, const char *delim) {
  static char *last = S21_NULL;
  char *pointer = S21_NULL;

  if (str != S21_NULL) last = str;

  while (*last != '\0' && s21_strchr(delim, *last) != S21_NULL) last++;

  if (*last != '\0') {
    pointer = last;
    while (*last != '\0' && s21_strchr(delim, *last) == S21_NULL) last++;

    if (*last != '\0') *last++ = '\0';
  }

  return pointer;
}
