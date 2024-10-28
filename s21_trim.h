//
// Created by Peanut Grodd on 11/20/23.
//

#ifndef C2_S21_STRINGPLUS_1_S21_TRIM_H
#define C2_S21_STRINGPLUS_1_S21_TRIM_H

#define MAX_ASCII_LENGTH 256

#include "common.h"
#include "s21_memset.h"
#include "s21_strcspn.h"
#include "s21_strlen.h"
#include "s21_strstr.h"

void *s21_trim(const char *src, const char *trim_chars);
int was_in_array(char sym, const char *array, s21_size_t length);

#endif  // C2_S21_STRINGPLUS_1_S21_TRIM_H
