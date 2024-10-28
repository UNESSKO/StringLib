//
// Created by Peanut Grodd on 11/20/23.
//

#include "s21_trim.h"

int was_in_array(const char sym, const char *array, const s21_size_t length) {
  int result = 0;
  for (s21_size_t i = 0; i < length; i++) {
    if (sym == array[i]) {
      result = 1;
      break;
    }
  }
  return result;
}

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t length = 0;
  s21_size_t count = 0;
  int finish = 0;
  while (str1[length] != '\0' && !finish) {
    s21_size_t was = 0;
    for (s21_size_t i = 0; str2[i] != '\0' && !was; ++i) {
      if (str1[length] == str2[i]) {
        count++;
        was = 1;
      } else if (!was_in_array(str1[length], str2, s21_strlen(str2))) {
        finish = 1;
        break;
      }
    }
    ++length;
  }
  return count;
}

s21_size_t s21_strspn_rev(const char *str1, const char *str2) {
  s21_size_t length = s21_strlen(str1) - 1;
  s21_size_t count = 0;
  int finish = 0;
  while (length != 0 && !finish) {
    s21_size_t was = 0;
    for (s21_size_t i = 0; str2[i] != '\0' && !was; ++i) {
      if (str1[length] == str2[i]) {
        count++;
        was = 1;
      } else if (!was_in_array(str1[length], str2, s21_strlen(str2))) {
        finish = 1;
        break;
      }
    }
    --length;
  }
  return count;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *res = S21_NULL;
  if (src != S21_NULL && trim_chars != S21_NULL) {
    s21_size_t src_len = s21_strlen(src);
    if (s21_strlen(trim_chars) == 0) {
      trim_chars = "\n\t\r ";
    }
    s21_size_t start = s21_strspn(src, trim_chars);
    s21_size_t end = s21_strspn_rev(src, trim_chars);
    s21_size_t len = src_len - start - end;
    if (src_len != start) {
      res = create_string(src);
      s21_size_t i = 0;
      while (i < len) {
        res[i] = (char)src[i + start];
        i++;
      }
      res[len] = '\0';
    } else
      res = create_string("");
  }
  return res;
}
