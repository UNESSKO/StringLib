//
// Created by Filch Lenwood on 11/20/23.
//
#include "s21_strchr.h"

char *s21_strchr(const char *str, int c) {
  char *result = S21_NULL;
  if (str != S21_NULL) {
    while (*str != '\0') {
      if (*str == c) {
        result = (char *)str;
        break;
      }
      str++;
    }
    if (*str == c) result = (char *)str;
  }
  return result;
}
