#ifndef C2_S21_STRINGPLUS_1_S21_SPRINTF_H
#define C2_S21_STRINGPLUS_1_S21_SPRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "common.h"
#include "s21_strlen.h"

#define MAX_STR_LEN 500

#define TO_STRING(num, str, space, zero_flag, plus, precision)          \
  _Generic((num), int                                                   \
           : int_to_string, unsigned int                                \
           : unsigned_int_to_string, short                              \
           : short_to_string, unsigned short                            \
           : unsigned_short_to_string, long                             \
           : long_to_string, unsigned long                              \
           : unsigned_long_to_string)(num, str, space, zero_flag, plus, \
                                      precision)

#define TO_OCTAL(decimalNumber, str, pref, space, zero_flag) \
  _Generic((decimalNumber), unsigned int                     \
           : int_to_octal, unsigned short                    \
           : short_to_octal, unsigned long                   \
           : long_to_octal)(decimalNumber, str, pref, space, zero_flag)

#define TO_HEX(decimalNumber, str, size, pref, space, zero_flag) \
  _Generic((decimalNumber), unsigned int                         \
           : int_to_hex, unsigned short                          \
           : short_to_hex, unsigned long                         \
           : long_to_hex)(decimalNumber, str, size, pref, space, zero_flag)

#define FLOAT(num, str, precision, space, zero_flag, plus, e_flag, pref)  \
  _Generic((num), double                                                  \
           : float_to_string, long double                                 \
           : long_float_to_string)(num, str, precision, space, zero_flag, \
                                   plus, e_flag, pref)

#define EXP(num, str, size, precision, space, zero_flag, plus)              \
  _Generic((num), double                                                    \
           : exponential, long double                                       \
           : long_exponential)(num, str, size, precision, space, zero_flag, \
                               plus)

#define G_FLAG(num, precision, str, size, space, zero_flag, plus, pref)      \
  _Generic((num), double                                                     \
           : g_flag, long double                                             \
           : long_g_flag)(num, precision, str, size, space, zero_flag, plus, \
                          pref)

int get_length(int num);

int unsigned_get_length(unsigned int num);

int long_get_length(unsigned long int num);

int type_plus(char *str, int plus, int negative, int is_not_changed_minus);

int space_input(int num, char *str, int zero_falg, int *negative,
                int precision);

int input(int plus, char *str, int zero_flag, int *negative, int space, int len,
          int precision);

int int_to_string(int num, char *str, int space, int zero_flag, int plus,
                  int precision);

int unsigned_int_to_string(unsigned int num, char *str, int space,
                           int zero_flag, int plus, int precision);

int short_to_string(short int num, char *str, int space, int zero_flag,
                    int plus, int precision);

int unsigned_short_to_string(unsigned short int num, char *str, int space,
                             int zero_flag, int plus, int precision);

int long_to_string(long int num, char *str, int space, int zero_flag, int plus,
                   int precision);

int unsigned_long_to_string(unsigned long int num, char *str, int space,
                            int zero_flag, int plus, int precision);

int float_to_string(double num, char *str, int precision, int space,
                    int zero_flag, int plus, int e_flag, int pref);

int long_float_to_string(long double num, char *str, int precision, int space,
                         int zero_flag, int plus, int e_flag, int pref);

int mant(double *mantissa);

int long_mant(long double *mantissa);

int nines(long double num);

int exponential(double num, char *str, int size, int precision, int space,
                int zero_flag, int plus);

int long_exponential(long double num, char *str, int size, int precision,
                     int space, int zero_flag, int plus);

int g_flag(double num, int precision, char *str, int size, int space,
           int zero_flag, int plus, int pref);

int long_g_flag(long double num, int precision, char *str, int size, int space,
                int zero_flag, int plus, int pref);

int zero_to_str(char *str, int *pref);

void reverse_num(int start, int end, char result[]);

int input_pref(int sys, int size, char *str);

int int_to_octal(unsigned int decimalNumber, char *str, int pref, int space,
                 int zero_flag);

int short_to_octal(unsigned short int decimalNumber, char *str, int pref,
                   int space, int zero_flag);

int long_to_octal(unsigned long int decimalNumber, char *str, int pref,
                  int space, int zero_flag);

int inputs(int zero_flag, int space, char *str, int negative, int size,
           int pref, int index, int temp);

int int_to_hex(unsigned int decimalNumber, char *str, int size, int pref,
               int space, int zero_flag);

int short_to_hex(unsigned short int decimalNumber, char *str, int size,
                 int pref, int space, int zero_flag);

int long_to_hex(unsigned long int decimalNumber, char *str, int size, int pref,
                int space, int zero_flag);

int parser(char flag, va_list args, char **str, int precision, int pref,
           int count, int space, int zero_flag, int plus, int type);

int s21_sprintf(char *str, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

void format_plus_and_minus(const char **format, int *zero_flag, int *plus);

void format_numbers(const char **format, int *zero_flag, int *space);

void format_dot(const char **format, int *precision, va_list args);

void format_h_l_L(const char **format, int *type);

void iterate_format_string(const char *format, va_list args, int *length,
                           char **str);

void parse_d_i(va_list args, char **str, int type, int space, int zero_flag,
               int plus, int *length, int precision);

void parse_u(va_list args, char **str, int type, int space, int zero_flag,
             int plus, int *length, int precision);

void parse_e_E(va_list args, char **str, int type, int space, int zero_flag,
               int plus, int precision, int *length, char flag);

void parse_f(va_list args, char **str, int type, int space, int zero_flag,
             int plus, int precision, int *length, int pref);

void parse_g_G(va_list args, char **str, int type, int space, int zero_flag,
               int plus, int precision, int *length, char flag, int pref);

void parse_o(va_list args, char **str, int type, int space, int zero_flag,
             int pref, int *length);

void parse_x_X(va_list args, char **str, int type, int space, int zero_flag,
               int pref, int *length, char flag);

void parse_s(va_list args, char **str, int *length, int type, int space,
             int zero_flag, int precision);

void parse_c(va_list args, char **str, int *length, int type, int space,
             int zero_flag);

void parse_p(va_list args, char **str, int *length, int space, int zero_flag);

#endif  // C2_S21_STRINGPLUS_1_S21_SPRINTF_H