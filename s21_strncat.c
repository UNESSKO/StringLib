#include "s21_strncat.h"

char *s21_strncat(char *destination, const char *append, s21_size_t n) {
  char *end_of_destination = destination;
  while (*end_of_destination != '\0') {
    ++end_of_destination;
  }

  while (*append != '\0' && n > 0) {
    *end_of_destination = *append;
    ++end_of_destination;
    ++append;
    --n;
  }

  *end_of_destination = '\0';

  return destination;
}
