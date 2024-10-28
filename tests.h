
//
// Created by Peanut Grodd on 11/17/23.
//

#ifndef C2_S21_STRINGPLUS_1_TESTS_H
#define C2_S21_STRINGPLUS_1_TESTS_H

#include "common.h"
#include "s21_string.h"
#define PI 3.1415926535897932

int s21_strncmp_test();
int run_s21_strlen_tests();
int s21_strerror_tests();
int s21_memset_test();
int run_to_upper_tests();
int run_s21_strncat_tests();
int s21_strncpy_test();
int s21_to_lower_test();
int s21_memcpy_test();
int s21_strstr_test();
int s21_strcspn_test();
int s21_strchr_test();
int s21_strrchr_test();
int s21_sprintf_test();

int s21_strtok_test();
int s21_strpbrk_test();
int s21_memcmp_tests();
int s21_insert_test();
int s21_trim_tests();
void generic_trim_tests(const char *src, const char *trim_chars,
                        const char *check);
int s21_memchr_test();
void generic_memchr_test(const char *str, char chr, size_t n);

#endif  // C2_S21_STRINGPLUS_1_TESTS_H