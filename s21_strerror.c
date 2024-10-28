#include "s21_strerror.h"

char *s21_strerror(int s21_errnum) {
  char *error_messages[] = ERRORS;
  char *result = 0;
  if (s21_errnum >= 0 && s21_errnum < MAX_ERRNO) {
    result = error_messages[s21_errnum];
  } else {
    static char unknown_error[150];
    if (OS == 1) {
      s21_sprintf(unknown_error, "Unknown error: %d", s21_errnum);
    } else if (OS == 2) {
      s21_sprintf(unknown_error, "Unknown error %d", s21_errnum);
    }
    result = unknown_error;
  }
  return result;
}