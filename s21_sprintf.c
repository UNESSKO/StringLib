#include "s21_sprintf.h"

int get_length(int num) {
  int length = 0;

  if (num == 0) length = 1;
  while (num != 0) {
    length++;
    num /= 10;
  }

  return length;
}

int unsigned_get_length(unsigned int num) {
  int length = 0;

  if (num == 0) length = 1;
  while (num != 0) {
    length++;
    num /= 10;
  }

  return length;
}

int long_get_length(unsigned long int num) {
  int length = 0;

  if (num == 0) length = 1;
  while (num != 0) {
    length++;
    num /= 10;
  }

  return length;
}

int type_plus(char *str, int plus, int negative, int is_not_changed_minus) {
  int length = 0;
  if ((plus == 1 || plus == 2) && !negative && is_not_changed_minus) {
    if (plus == 1) {
      *str = '+';
      length++;
    } else {
      *str++ = ' ';
      length++;
    }
  }

  return length;
}

int space_input(int num, char *str, int zero_flag, int *negative,
                int precision) {
  int length = 0;
  char *start = str;

  for (int i = 0; i < num; ++i) {
    if (zero_flag == 1) {
      if (*negative == 1) {
        if (precision > 0) --i;
        *str++ = '-';
        *negative = 0;
      } else {
        *str = '0';
        str++;
      }
    } else {
      *str++ = ' ';
    }
    length++;
  }
  if (*negative == 1) {
    length++;
    *str++ = '-';
    *negative = 0;
  }

  *str = '\0';
  str = start;

  return length;
}

int input(int plus, char *str, int zero_flag, int *negative, int space, int len,
          int precision) {
  int length = 0;
  int added_length = 0;
  int temp = 0;
  int was_negative = *negative;
  if (precision > len)
    temp = precision;
  else
    temp = len;
  int sym = 0;
  if (*negative || plus > 0) sym = 1;

  if (precision > 0) {
    if (plus && zero_flag != -1) {
      added_length = space_input(space - temp - sym, str + length, zero_flag,
                                 negative, precision);
      length += added_length;
      str += added_length;
      added_length = type_plus(str, plus, *negative, was_negative == *negative);
      length += added_length;
      str += added_length;
      added_length = space_input(precision - len, str, 1, negative, precision);
      length += added_length;
      str += added_length;
    } else if (zero_flag == 1) {
      length += space_input(space - temp, str, zero_flag, negative, precision);
    } else {
      if (plus) {
        if (zero_flag == 1) {
          added_length =
              type_plus(str, plus, *negative, was_negative == *negative);
          length += added_length;
          str += added_length;
          added_length =
              space_input(space - precision - len, str, 0, negative, precision);
          length += added_length;
          str += added_length;
        } else if (zero_flag == 0) {
          added_length =
              space_input(space - precision - len, str, 0, negative, precision);
          length += added_length;
          str += added_length;
          added_length =
              type_plus(str, plus, *negative, was_negative == *negative);
          length += added_length;
          str += added_length;
        } else {
          added_length =
              type_plus(str, plus, *negative, was_negative == *negative);
          length += added_length;
          str += added_length;
        }
      } else if (zero_flag != -1) {
        added_length =
            type_plus(str, plus, *negative, was_negative == *negative);
        length += added_length;
        str += added_length;
        added_length =
            space_input(space - temp - *negative, str, 0, negative, precision);
        length += added_length;
        str += added_length;
      }
      added_length = space_input(precision - len, str, 1, negative, precision);
      length += added_length;
      str += added_length;
    }
  } else {
    if (plus) {
      if (zero_flag == 1) {
        length += type_plus(str, plus, *negative, was_negative == *negative);
        length += space_input(space - len - length, str + length, zero_flag,
                              negative, precision);
      } else if (zero_flag != -1) {
        length +=
            space_input(space - len - sym, str, zero_flag, negative, precision);
        length +=
            type_plus(str + length, plus, *negative, was_negative == *negative);
      } else {
        length +=
            type_plus(str + length, plus, *negative, was_negative == *negative);
      }
    } else if (zero_flag == 1) {
      length += space_input(space - len, str, zero_flag, negative, precision);
    } else if (zero_flag != -1) {
      length +=
          space_input(space - len - sym, str, zero_flag, negative, precision);
    }
  }

  return length;
}

int int_to_string(int num, char *str, int space, int zero_flag, int plus,
                  int precision) {
  int length = 0;
  int temp = num;
  int negative = 0;
  int num_len = get_length(num);

  if (num < 0) {
    negative = 1;
    num = -num;
  }

  length += input(plus, str, zero_flag, &negative, space, num_len, precision);

  length += num_len;
  if (negative && precision > 0) length++;
  if (temp < 0 && precision < 0 && (!zero_flag || zero_flag == -1)) {
    str += length + negative;
    length += negative;
  } else if (temp < 0 && precision > 0 && space > 0)
    str += length;
  else
    str += length;
  *str = '\0';

  do {
    *(--str) = num % 10 + '0';
    num /= 10;
  } while (num != 0);

  if (negative && precision < 1 && !zero_flag) {
    *(--str) = '-';
    negative = 0;
    str += length;
  } else if (negative && precision < 1 && zero_flag == -1) {
    *(--str) = '-';
    negative = 0;
    str += length;
  } else if (negative) {
    *(--str) = '-';
    str += length;
  } else {
    str += num_len;
  }

  if (zero_flag == -1) {
    length += space_input(space - length, str, 0, &negative, precision);
  }
  return length;
}

int unsigned_int_to_string(unsigned int num, char *str, int space,
                           int zero_flag, int plus, int precision) {
  int length = 0;
  int negative = 0;
  plus = 0;
  int num_len = unsigned_get_length(num);

  length += input(plus, str, zero_flag, &negative, space, num_len, precision);

  length += num_len;
  str += length;
  *str = '\0';

  do {
    *(--str) = num % 10 + '0';
    num /= 10;
  } while (num != 0);

  if (zero_flag == -1) {
    length +=
        space_input(space - length, str + num_len, 0, &negative, precision);
  }

  return length;
}

int short_to_string(short int num, char *str, int space, int zero_flag,
                    int plus, int precision) {
  int length = 0;
  short int temp = num;
  int negative = 0;
  int num_len = get_length(num);

  if (num < 0) {
    negative = 1;
    num = -num;
  }

  length += input(plus, str, zero_flag, &negative, space, num_len, precision);

  length += num_len;
  if (negative && precision > 0) length++;
  if (temp < 0 && precision < 0 && (!zero_flag || zero_flag == -1)) {
    str += length + negative;
    length += negative;
  } else if (temp < 0 && precision > 0 && space > 0)
    str += length;
  else
    str += length;
  *str = '\0';

  do {
    *(--str) = num % 10 + '0';
    num /= 10;
  } while (num != 0);

  if (negative && precision < 1 && !zero_flag) {
    *(--str) = '-';
    negative = 0;
    str += length;
  } else if (negative && precision < 1 && zero_flag == -1) {
    *(--str) = '-';
    negative = 0;
    str += length;
  } else if (negative) {
    *(--str) = '-';
    str += length;
  } else {
    str += num_len;
  }

  if (zero_flag == -1) {
    length += space_input(space - length, str, 0, &negative, precision);
  }
  return length;
}

int unsigned_short_to_string(unsigned short int num, char *str, int space,
                             int zero_flag, int plus, int precision) {
  int length = 0;
  int negative = 0;
  plus = 0;
  unsigned short int num_len = unsigned_get_length(num);

  length += input(plus, str, zero_flag, &negative, space, num_len, precision);

  length += num_len;
  str += length;
  *str = '\0';

  do {
    *(--str) = num % 10 + '0';
    num /= 10;
  } while (num != 0);

  if (zero_flag == -1) {
    length +=
        space_input(space - length, str + length, 0, &negative, precision);
  }

  return length;
}

int long_to_string(long int num, char *str, int space, int zero_flag, int plus,
                   int precision) {
  int length = 0;
  long long temp = num;
  int negative = 0;
  int revers = 0;
  if (num < 0) {
    num *= -1;
    revers = 1;
  }
  long int num_len = long_get_length(num);
  if (revers) num *= -1;
  if (num < 0) {
    negative = 1;
    num = -num;
  }

  length += input(plus, str, zero_flag, &negative, space, num_len, precision);

  length += num_len;
  if (negative && precision > 0) length++;
  if (temp < 0 && precision < 0 && (!zero_flag || zero_flag == -1)) {
    str += length + negative;
    length += negative;
  } else if (temp < 0 && precision > 0 && space > 0)
    str += length;
  else
    str += length;
  *str = '\0';

  do {
    *(--str) = num % 10 + '0';
    num /= 10;
  } while (num != 0);

  if (negative && precision < 1 && !zero_flag) {
    *(--str) = '-';
    negative = 0;
    str += length;
  } else if (negative && precision < 1 && zero_flag == -1) {
    *(--str) = '-';
    negative = 0;
    str += length;
  } else if (negative) {
    *(--str) = '-';
    str += length;
  } else {
    str += num_len;
  }

  if (zero_flag == -1) {
    length += space_input(space - length, str, 0, &negative, precision);
  }
  return length;
}

int unsigned_long_to_string(unsigned long int num, char *str, int space,
                            int zero_flag, int plus, int precision) {
  int length = 0;
  int negative = 0;
  plus = 0;
  int num_len = long_get_length(num);

  length += input(plus, str, zero_flag, &negative, space, num_len, precision);

  length += num_len;
  str += length;
  *str = '\0';

  do {
    *(--str) = num % 10 + '0';
    num /= 10;
  } while (num != 0);

  if (zero_flag == -1) {
    length +=
        space_input(space - length, str + length, 0, &negative, precision);
  }

  return length;
}

int float_to_string(double num, char *str, int precision, int space,
                    int zero_flag, int plus, int e_flag, int pref) {
  int length = 0;
  int negative = 0;
  if (precision == -1) precision = 6;

  if (num < 0) {
    if (space > 0 && zero_flag != -1) {
      length += space_input(space - precision - get_length(num) - 2, str,
                            zero_flag, &negative, 0);
      space = 0;
    }
    str += length;
    *str++ = '-';
    length++;
    num = -num;
    plus = 0;
  }

  long int intPart = (long int)num;
  double fractionalPart = num - intPart;
  if (precision == 0) {
    int a = fractionalPart * 10;
    if (a >= 5) intPart++;
  }
  int temp = 0;
  if (precision != 0) temp = 1;
  if (zero_flag != -1) {
    length +=
        TO_STRING(intPart, str, space - precision - temp, zero_flag, plus, 0);
  } else {
    length += TO_STRING(intPart, str, 0, zero_flag, plus, 0);
  }

  while (*str != '\0') str++;

  if (precision != 0 || pref != 0) {
    *str++ = '.';
    length++;
  }

  for (int i = 0; i < precision; i++) {
    fractionalPart *= 10;
  }

  fractionalPart += 0.5;

  for (int i = 0; i < precision; ++i) {
    fractionalPart /= 10;
  }

  for (int i = 0; i < precision; ++i) {
    fractionalPart *= 10;
    unsigned long int digit = (int)fractionalPart;
    *str++ = digit + '0';
    fractionalPart -= digit;
    length++;
  }

  *str = '\0';
  if (zero_flag == -1 && !e_flag) {
    length += space_input(space - length, str, zero_flag, &negative, 0);
  }

  return length;
}

int long_float_to_string(long double num, char *str, int precision, int space,
                         int zero_flag, int plus, int e_flag, int pref) {
  int length = 0;
  int negative = 0;
  if (precision == -1) precision = 6;

  if (num < 0) {
    if (space > 0 && zero_flag != -1) {
      length += space_input(space - precision - get_length(num) - 2, str,
                            zero_flag, &negative, 0);
      space = 0;
    }
    str += length;
    *str++ = '-';
    length++;
    num = -num;
    plus = 0;
  }

  int intPart = (int)num;
  long double fractionalPart = num - intPart;

  if (zero_flag != -1) {
    length += TO_STRING(intPart, str, space - precision - 1 - plus, zero_flag,
                        plus, 0);
  } else {
    length += TO_STRING(intPart, str, 0, zero_flag, plus, 0);
  }

  while (*str != '\0') str++;

  if (precision != 0 || pref != 0) {
    *str++ = '.';
    length++;
  }

  for (int i = 0; i < precision; i++) {
    fractionalPart *= 10;
  }

  fractionalPart += 0.5;

  for (int i = 0; i < precision; ++i) {
    fractionalPart /= 10;
  }

  for (int i = 0; i < precision; ++i) {
    fractionalPart *= 10;
    int digit = (int)fractionalPart;
    *str++ = digit + '0';
    fractionalPart -= digit;
    length++;
  }

  *str = '\0';
  if (zero_flag == -1 && !e_flag) {
    length += space_input(space - length, str, zero_flag, &negative, 0);
  }

  return length;
}

int mant(double *mantissa) {
  int exponent = 0;
  while (*mantissa >= 10.0 || *mantissa <= -10.0) {
    *mantissa /= 10.0;
    exponent++;
  }

  while (*mantissa < 1.0 && *mantissa > -1.0 && *mantissa != 0.0) {
    *mantissa *= 10.0;
    exponent--;
  }

  return exponent;
}

int long_mant(long double *mantissa) {
  int exponent = 0;
  while (*mantissa >= 10.0 || *mantissa <= -10.0) {
    *mantissa /= 10.0;
    exponent++;
  }

  while (*mantissa < 1.0 && *mantissa > -1.0 && *mantissa != 0.0) {
    *mantissa *= 10.0;
    exponent--;
  }

  return exponent;
}

int nines(long double num) {
  long long integerPart = (long long)num;
  long double fractionalPart = num - (long double)integerPart;

  int count = 0;

  while (fractionalPart > 0.0 && count < 7) {
    fractionalPart *= 10;
    int digit = (int)fractionalPart;
    if (digit == 9) {
      count++;
    } else {
      break;
    }
    fractionalPart -= digit;
  }

  return count;
}

int long_exponential(long double num, char *str, int size, int precision,
                     int space, int zero_flag, int plus) {
  int length = 0;

  if (num != 0) {
    long double mantissa = num;
    int exponent = long_mant(&mantissa);
    if (nines(mantissa) >= 7) {
      for (int i = 0; i < precision; i++) {
        mantissa *= 10;
      }

      mantissa += 0.5;

      for (int i = 0; i < precision; ++i) {
        mantissa /= 10;
      }
      if (mantissa >= 10) {
        mantissa -= 9;
        exponent++;
      }
    }
    length += FLOAT(mantissa, str, precision, space, zero_flag, plus, 1, 0);
    str = &str[length];

    if (precision != 1) {
      if (size == 1)
        *str++ = 'E';
      else
        *str++ = 'e';
      if (exponent >= 0)
        *str++ = '+';
      else {
        *str++ = '-';
        exponent = -exponent;
      }
      if (exponent < 10) {
        *str++ = '0';
        length++;
      }
      length += 2;
      space = 0;
      zero_flag = 0;
      plus = 0;
      length += TO_STRING(exponent, str, space, zero_flag, plus, 0);
    }
  } else {
    long double mantissa = 0.0;
    int exponent = 0;
    length += FLOAT(mantissa, str, precision, space, zero_flag, plus, 1, 0);
    str = &str[length];
    if (size == 1)
      *str++ = 'E';
    else
      *str++ = 'e';
    *str++ = '+';
    *str++ = '0';
    length++;
    length += 2;
    length += TO_STRING(exponent, str, space, zero_flag, plus, 0);
  }

  return length;
}

int exponential(double num, char *str, int size, int precision, int space,
                int zero_flag, int plus) {
  int length = 0;
  if (num != 0) {
    double mantissa = num;

    int exponent = mant(&mantissa);
    if (nines(mantissa) >= 7) {
      for (int i = 0; i < precision; i++) {
        mantissa *= 10;
      }

      mantissa += 0.5;

      for (int i = 0; i < precision; ++i) {
        mantissa /= 10;
      }
      if (mantissa >= 10) {
        mantissa -= 9;
        exponent++;
      }
    }

    length +=
        FLOAT(mantissa, str, precision, space - plus, zero_flag, plus, 1, 0);
    str = &str[length];

    if (precision != 1) {
      if (size == 1)
        *str++ = 'E';
      else
        *str++ = 'e';
      if (exponent >= 0)
        *str++ = '+';
      else {
        *str++ = '-';
        exponent = -exponent;
      }
      if (exponent < 10) {
        *str++ = '0';
        length++;
      }
      length += 2;
      // space = 0;
      zero_flag = 0;
      plus = 0;
      length += TO_STRING(exponent, str, space - length, -1, plus, 0);
    }
  } else {
    double mantissa = 0.0;
    int exponent = 0;
    length += FLOAT(mantissa, str, precision, space, zero_flag, plus, 1, 0);
    str = &str[length];
    if (size == 1)
      *str++ = 'E';
    else
      *str++ = 'e';
    *str++ = '+';
    *str++ = '0';
    length++;
    length += 2;
    length += TO_STRING(exponent, str, space, zero_flag, plus, 0);
  }

  return length;
}

int g_flag(double num, int precision, char *str, int size, int space,
           int zero_flag, int plus, int pref) {
  int length = 0;
  int copy = precision;

  if (num != 0) {
    double temp = num;
    int exp = mant(&temp);
    if (copy == -1) copy = 6;
    if (copy > exp && exp >= -4) {
      if (precision == -1) precision = 6;
      precision -= exp + 1;
      length = FLOAT(num, str, precision, space, zero_flag, plus, 0, 0);
    } else {
      if (precision == -1) precision = 5;
      length = EXP(num, str, size, precision, space, zero_flag, plus);
    }
  } else {
    int neg = 0;
    *str++ = '0';
    if (pref) {
      *str++ = '.';
      *str++ = '0';
      length += 2;
    }
    length++;
    length += space_input(space - length, str, zero_flag, &neg, 0);
  }

  return length;
}

int long_g_flag(long double num, int precision, char *str, int size, int space,
                int zero_flag, int plus, int pref) {
  int length = 0;
  int copy = precision;

  if (num != 0) {
    long double temp = num;
    int exp = long_mant(&temp);
    if (copy == -1) copy = 6;
    if (copy > exp && exp >= -4) {
      if (precision == -1) precision = 6;
      precision -= exp + 1;
      length = FLOAT(num, str, precision, space, zero_flag, plus, 0, 0);
    } else {
      if (precision == -1) precision = 5;
      length = EXP(num, str, size, precision, space, zero_flag, plus);
    }
  } else {
    int neg = 0;
    *str++ = '0';
    if (pref) {
      *str++ = '.';
      *str++ = '0';
      length += 2;
    }
    length++;
    length += space_input(space - length, str, zero_flag, &neg, 0);
  }

  return length;
}

int zero_to_str(char *str, int *pref) {
  *(str++) = '0';
  *pref = 0;
  *str = '\0';

  return 1;
}

void reverse_num(int start, int end, char result[]) {
  while (start < end) {
    char temp = result[start];
    result[start] = result[end];
    result[end] = temp;
    start++;
    end--;
  }
}

int input_pref(int sys, int size, char *str) {
  int length = 0;

  if (sys == 8) {
    *str++ = '0';
    length++;
  } else if (sys == 16) {
    if (size == 1) {
      *str++ = '0';
      *str++ = 'X';
      length += 2;
    } else {
      *str++ = '0';
      *str++ = 'x';
      length += 2;
    }
  }

  return length;
}

int int_to_octal(unsigned int decimalNumber, char *str, int pref, int space,
                 int zero_flag) {
  int length = 0;
  int negative = 0;
  int index = 0;
  unsigned int save = decimalNumber;
  char octalResult[50];

  if (decimalNumber == 0) {
    length += zero_to_str(str, &pref);
    str += length;
    length += space_input(space - length - pref, str, zero_flag, &negative, 0);
  } else {
    while (decimalNumber > 0) {
      unsigned int remainder = decimalNumber % 8;
      octalResult[index++] = remainder + '0';
      decimalNumber /= 8;
    }
    octalResult[index] = '\0';

    if (zero_flag == 0)
      length += space_input(space - index - pref, str, zero_flag, &negative, 0);
    str += length;

    if (pref) {
      length += input_pref(8, 0, str);
      str += length;
    }

    if (zero_flag == 1)
      length += space_input(space - index - pref, str, zero_flag, &negative, 0);

    int start = 0;
    int end = index - 1;
    reverse_num(start, end, octalResult);
  }

  if (save > 0) {
    if (!pref || zero_flag == 1) str += length;
    for (int i = 0; octalResult[i] != '\0'; ++i) {
      *str++ = octalResult[i];
      length++;
    }
  }

  if (zero_flag == -1)
    length += space_input(space - index - pref, str, zero_flag, &negative, 0);

  return length;
}

int short_to_octal(unsigned short int decimalNumber, char *str, int pref,
                   int space, int zero_flag) {
  int length = 0;
  int negative = 0;
  int index = 0;
  unsigned short int save = decimalNumber;
  char octalResult[50];

  if (decimalNumber == 0) {
    length += zero_to_str(str, &pref);
    str += length;
    length += space_input(space - length - pref, str, zero_flag, &negative, 0);
  } else {
    while (decimalNumber > 0) {
      unsigned int remainder = decimalNumber % 8;
      octalResult[index++] = remainder + '0';
      decimalNumber /= 8;
    }
    octalResult[index] = '\0';

    if (zero_flag == 0)
      length += space_input(space - index - pref, str, zero_flag, &negative, 0);
    str += length;

    if (pref) {
      length += input_pref(8, 0, str);
      str += length;
    }

    if (zero_flag == 1)
      length += space_input(space - index - pref, str, zero_flag, &negative, 0);

    int start = 0;
    int end = index - 1;
    reverse_num(start, end, octalResult);
  }

  if (save > 0) {
    if (!pref || zero_flag == 1) str += length;
    for (int i = 0; octalResult[i] != '\0'; ++i) {
      *str++ = octalResult[i];
      length++;
    }
  }

  if (zero_flag == -1)
    length += space_input(space - index - pref, str, zero_flag, &negative, 0);

  return length;
}

int long_to_octal(unsigned long int decimalNumber, char *str, int pref,
                  int space, int zero_flag) {
  int length = 0;
  int negative = 0;
  int index = 0;
  unsigned long int save = decimalNumber;
  char octalResult[1500];

  if (decimalNumber == 0) {
    length += zero_to_str(str, &pref);
    str += length;
    length += space_input(space - length - pref, str, zero_flag, &negative, 0);
  } else {
    while (decimalNumber > 0) {
      unsigned long int remainder = decimalNumber % 8;
      octalResult[index++] = remainder + '0';
      decimalNumber /= 8;
    }
    octalResult[index] = '\0';

    if (zero_flag == 0)
      length += space_input(space - index - pref, str, zero_flag, &negative, 0);
    str += length;

    if (pref) {
      length += input_pref(8, 0, str);
      str += length;
    }

    if (zero_flag == 1)
      length += space_input(space - index - pref, str, zero_flag, &negative, 0);

    int start = 0;
    int end = index - 1;
    reverse_num(start, end, octalResult);
  }

  if (save > 0) {
    if (!pref || zero_flag == 1) str += length;
    for (int i = 0; octalResult[i] != '\0'; ++i) {
      *str++ = octalResult[i];
      length++;
    }
  }

  if (zero_flag == -1)
    length += space_input(space - index - pref, str, zero_flag, &negative, 0);

  return length;
}

int inputs(int zero_flag, int space, char *str, int negative, int size,
           int pref, int index, int temp) {
  int length = 0;

  if (zero_flag == 0)
    length += space_input(space - index - temp, str, zero_flag, &negative, 0);
  str += length;
  if (pref) {
    length += input_pref(16, size, str);
    str += length;
  }
  if (zero_flag == 1)
    length += space_input(space - index - temp, str, zero_flag, &negative, 0);

  return length;
}

int int_to_hex(unsigned int decimalNumber, char *str, int size, int pref,
               int space, int zero_flag) {
  int length = 0;
  int temp = 0;
  if (pref) temp = 2;
  unsigned int save = decimalNumber;
  int negative = 0;
  char hexDigitsLower[] = "0123456789abcdef";
  char hexDigitsUpper[] = "0123456789ABCDEF";
  char *hexDigits = hexDigitsLower;

  if (size == 1) hexDigits = hexDigitsUpper;
  char hexResult[50];
  int index = 0;
  if (decimalNumber == 0) {
    length += zero_to_str(str, &pref);
    str += length;
    length += space_input(space - length - temp, str, zero_flag, &negative, 0);
  } else {
    while (decimalNumber > 0) {
      int remainder = decimalNumber % 16;
      hexResult[index++] = hexDigits[remainder];
      decimalNumber /= 16;
    }
    hexResult[index] = '\0';

    length += inputs(zero_flag, space, str, negative, size, pref, index, temp);

    int start = 0;
    int end = index - 1;
    reverse_num(start, end, hexResult);
  }

  if (save > 0) {
    str += length;
    for (int i = 0; hexResult[i] != '\0'; ++i) {
      *str++ = hexResult[i];
      length++;
    }
  }
  *str = '\0';

  if (zero_flag == -1)
    length += space_input(space - index - temp, str, zero_flag, &negative, 0);

  return length;
}

int short_to_hex(unsigned short int decimalNumber, char *str, int size,
                 int pref, int space, int zero_flag) {
  int length = 0;
  int temp = 0;
  if (pref) temp = 2;
  unsigned short int save = decimalNumber;
  int negative = 0;
  char hexDigitsLower[] = "0123456789abcdef";
  char hexDigitsUpper[] = "0123456789ABCDEF";
  char *hexDigits = hexDigitsLower;

  if (size == 1) hexDigits = hexDigitsUpper;
  char hexResult[50];
  int index = 0;
  if (decimalNumber == 0) {
    length += zero_to_str(str, &pref);
    str += length;
    length += space_input(space - length - temp, str, zero_flag, &negative, 0);
  } else {
    while (decimalNumber > 0) {
      int remainder = decimalNumber % 16;
      hexResult[index++] = hexDigits[remainder];
      decimalNumber /= 16;
    }
    hexResult[index] = '\0';

    length += inputs(zero_flag, space, str, negative, size, pref, index, temp);

    int start = 0;
    int end = index - 1;
    reverse_num(start, end, hexResult);
  }

  if (save > 0) {
    str += length;
    for (int i = 0; hexResult[i] != '\0'; ++i) {
      *str++ = hexResult[i];
      length++;
    }
  }
  *str = '\0';

  if (zero_flag == -1)
    length += space_input(space - index - temp, str, zero_flag, &negative, 0);

  return length;
}

int long_to_hex(unsigned long int decimalNumber, char *str, int size, int pref,
                int space, int zero_flag) {
  int length = 0;
  int temp = 0;
  if (pref) temp = 2;
  unsigned long int save = decimalNumber;
  int negative = 0;
  char hexDigitsLower[] = "0123456789abcdef";
  char hexDigitsUpper[] = "0123456789ABCDEF";
  char *hexDigits = hexDigitsLower;

  if (size == 1) hexDigits = hexDigitsUpper;
  char hexResult[50];
  int index = 0;
  if (decimalNumber == 0) {
    length += zero_to_str(str, &pref);
    str += length;
    length += space_input(space - length - temp, str, zero_flag, &negative, 0);
  } else {
    while (decimalNumber > 0) {
      int remainder = decimalNumber % 16;
      hexResult[index++] = hexDigits[remainder];
      decimalNumber /= 16;
    }
    hexResult[index] = '\0';

    length += inputs(zero_flag, space, str, negative, size, pref, index, temp);

    int start = 0;
    int end = index - 1;
    reverse_num(start, end, hexResult);
  }

  if (save > 0) {
    str += length;
    for (int i = 0; hexResult[i] != '\0'; ++i) {
      *str++ = hexResult[i];
      length++;
    }
  }
  *str = '\0';

  if (zero_flag == -1)
    length += space_input(space - index - temp, str, zero_flag, &negative, 0);

  return length;
}

void parse_d_i(va_list args, char **str, int type, int space, int zero_flag,
               int plus, int *length, int precision) {
  if (type == 0) {
    int int_arg = va_arg(args, int);
    *length = TO_STRING(int_arg, *str, space, zero_flag, plus, precision);
  } else if (type == 1) {
    short int int_arg = (short int)va_arg(args, int);
    *length = TO_STRING(int_arg, *str, space, zero_flag, plus, precision);
  } else if (type == 2) {
    long int int_arg = va_arg(args, long int);
    *length = TO_STRING(int_arg, *str, space, zero_flag, plus, precision);
  }
}

void parse_u(va_list args, char **str, int type, int space, int zero_flag,
             int plus, int *length, int precision) {
  if (type == 0) {
    unsigned int int_arg = va_arg(args, unsigned int);
    *length = TO_STRING(int_arg, *str, space, zero_flag, plus, precision);
  } else if (type == 1) {
    unsigned short int int_arg = (unsigned short int)va_arg(args, int);
    *length = TO_STRING(int_arg, *str, space, zero_flag, plus, precision);
  } else if (type == 2) {
    unsigned long int int_arg = va_arg(args, unsigned long int);
    *length = TO_STRING(int_arg, *str, space, zero_flag, plus, precision);
  }
}

void parse_e_E(va_list args, char **str, int type, int space, int zero_flag,
               int plus, int precision, int *length, char flag) {
  if (type == 0) {
    double double_arg = va_arg(args, double);
    *length = EXP(double_arg, *str, flag == 'e' ? 0 : 1, precision, space,
                  zero_flag, plus);
  } else if (type == 3) {
    long double double_arg = va_arg(args, long double);
    *length = EXP(double_arg, *str, flag == 'e' ? 0 : 1, precision, space,
                  zero_flag, plus);
  }
}

void parse_f(va_list args, char **str, int type, int space, int zero_flag,
             int plus, int precision, int *length, int pref) {
  if (type == 0) {
    double double_arg = va_arg(args, double);
    *length =
        FLOAT(double_arg, *str, precision, space, zero_flag, plus, 0, pref);
  } else if (type == 3) {
    long double double_arg = va_arg(args, long double);
    *length =
        FLOAT(double_arg, *str, precision, space, zero_flag, plus, 0, pref);
  }
}

void parse_g_G(va_list args, char **str, int type, int space, int zero_flag,
               int plus, int precision, int *length, char flag, int pref) {
  if (type == 0) {
    double double_arg = va_arg(args, double);
    *length = G_FLAG(double_arg, precision, *str, flag == 'g' ? 0 : 1, space,
                     zero_flag, plus, pref);
  } else if (type == 3) {
    long double double_arg = va_arg(args, long double);
    *length = G_FLAG(double_arg, precision, *str, flag == 'g' ? 0 : 1, space,
                     zero_flag, plus, pref);
  }
}

void parse_o(va_list args, char **str, int type, int space, int zero_flag,
             int pref, int *length) {
  if (type == 0) {
    unsigned int int_arg = va_arg(args, unsigned int);
    *length = TO_OCTAL(int_arg, *str, pref, space, zero_flag);
  } else if (type == 1) {
    unsigned short int int_arg = (unsigned short int)va_arg(args, int);
    *length = TO_OCTAL(int_arg, *str, pref, space, zero_flag);
  } else if (type == 2) {
    unsigned long int int_arg = va_arg(args, unsigned long int);
    *length = TO_OCTAL(int_arg, *str, pref, space, zero_flag);
  }
}

void parse_x_X(va_list args, char **str, int type, int space, int zero_flag,
               int pref, int *length, char flag) {
  if (type == 0) {
    unsigned int int_arg = va_arg(args, unsigned int);
    *length =
        TO_HEX(int_arg, *str, flag == 'x' ? 0 : 1, pref, space, zero_flag);
  } else if (type == 1) {
    unsigned short int int_arg = (unsigned short int)va_arg(args, int);
    *length =
        TO_HEX(int_arg, *str, flag == 'x' ? 0 : 1, pref, space, zero_flag);
  } else if (type == 2) {
    unsigned long int int_arg = va_arg(args, unsigned long int);
    *length =
        TO_HEX(int_arg, *str, flag == 'x' ? 0 : 1, pref, space, zero_flag);
  }
}

void parse_s(va_list args, char **str, int *length, int type, int space,
             int zero_flag, int precision) {
  int neg = 0;
  if (type == 0) {
    char *string_arg = va_arg(args, char *);
    if (string_arg == S21_NULL) {
      string_arg = "(null)";
    }
    int temp = 0;
    const s21_size_t size = s21_strlen(string_arg);
    if (size < (s21_size_t)precision) {
      temp = (int)size;
    } else {
      temp = precision;
    }
    if (space > 0 && zero_flag != -1) {
      *length += space_input(space - temp, *str, 0, &neg, 0);
      *str += *length;
    }
    int i = 0;
    while (i < temp) {
      *(*str) = *string_arg++;
      (*str)++;
      (*length)++;
      **str = '\0';
      i++;
    }
    if (space > 0 && zero_flag == -1)
      *length += space_input(space - *length, *str, 0, &neg, 0);
  } else if (type == 2) {
    wchar_t *wide_str = va_arg(args, wchar_t *);
    while (*wide_str != L'\0') {
      wchar_t wide_char = *wide_str;

      if (wide_char <= 0x7F) {
        (*str)[(*length)++] = (char)wide_char;
      } else if (wide_char <= 0x7FF) {
        (*str)[(*length)++] = (char)(0xC0 | (wide_char >> 6));
        (*str)[(*length)++] = (char)(0x80 | (wide_char & 0x3F));
      } else if (wide_char <= 0xFFFF) {
        (*str)[(*length)++] = (char)(0xE0 | (wide_char >> 12));
        (*str)[(*length)++] = (char)(0x80 | ((wide_char >> 6) & 0x3F));
        (*str)[(*length)++] = (char)(0x80 | (wide_char & 0x3F));
      }
      wide_str++;
    }
    (*str)[*length] = '\0';
  }
}

void parse_c(va_list args, char **str, int *length, int type, int space,
             int zero_flag) {
  int neg = 0;
  if (type == 0) {
    char char_arg = va_arg(args, int);
    if (space > 0 && zero_flag != -1) {
      *length += space_input(space - 1, *str, 0, &neg, 0);
      *str += *length;
    }
    **str = char_arg;
    (*str)++;
    (*length)++;
    **str = '\0';
    if (space > 0 && zero_flag == -1)
      *length += space_input(space - 1, *str, 0, &neg, 0);
  } else if (type == 2) {
    wchar_t long_char_arg = va_arg(args, wchar_t);
    if (space > 0 && zero_flag != -1) {
      *length += space_input(space - 1, *str, 0, &neg, 0);
      //(*str) += *length;
    }

    if (long_char_arg <= 0x7F) {
      (*str)[(*length)++] = (char)long_char_arg;
      (*str) += *length;
    } else if (long_char_arg <= 0x7FF) {
      (*str)[(*length)++] = (char)(0xC0 | (long_char_arg >> 6));
      (*str)[(*length)++] = (char)(0x80 | (long_char_arg & 0x3F));
      (*str) += *length;
    } else if (long_char_arg <= 0xFFFF) {
      (*str)[(*length)++] = (char)(0xE0 | (long_char_arg >> 12));
      (*str)[(*length)++] = (char)(0x80 | ((long_char_arg >> 6) & 0x3F));
      (*str)[(*length)++] = (char)(0x80 | (long_char_arg & 0x3F));
      (*str) += *length;
    }
    if (space > 0 && zero_flag == -1)
      *length += space_input(space - 1, *str, 0, &neg, 0);
  }
}

void parse_p(va_list args, char **str, int *length, int space, int zero_flag) {
  void *ptr = va_arg(args, void *);
  if (ptr == S21_NULL) {
    char *nil_str = "(nil)";
    while (*nil_str != '\0') {
      *(*str) = *nil_str++;
      (*str)++;
      (*length)++;
      **str = '\0';
    }
  } else {
    unsigned long int decimal = (unsigned long int)ptr;
    *length = TO_HEX(decimal, *str, 0, 1, space, zero_flag);
  }
}

int parser(char flag, va_list args, char **str, int precision, int pref,
           int count, int space, int zero_flag, int plus, int type) {
  int length = 0;

  if (flag == 'c') {
    parse_c(args, str, &length, type, space, zero_flag);

  } else if (flag == 'd' || flag == 'i') {
    parse_d_i(args, str, type, space, zero_flag, plus, &length, precision);
  } else if (flag == 'u') {
    parse_u(args, str, type, space, zero_flag, plus, &length, precision);
  } else if (flag == 'e' || flag == 'E') {
    parse_e_E(args, str, type, space, zero_flag, plus, precision, &length,
              flag);
  } else if (flag == 'f') {
    parse_f(args, str, type, space, zero_flag, plus, precision, &length, pref);
  } else if (flag == 'g' || flag == 'G') {
    parse_g_G(args, str, type, space, zero_flag, plus, precision, &length, flag,
              pref);
  } else if (flag == 'o') {
    parse_o(args, str, type, space, zero_flag, pref, &length);
  } else if (flag == 's') {
    parse_s(args, str, &length, type, space, zero_flag, precision);
  } else if (flag == 'x' || flag == 'X') {
    parse_x_X(args, str, type, space, zero_flag, pref, &length, flag);
  } else if (flag == 'p') {
    parse_p(args, str, &length, space, zero_flag);
  } else if (flag == 'n') {
    int *ptr = va_arg(args, int *);
    int temp = 0;
    if (ptr != S21_NULL && count != 0) {
      temp = count;
      *ptr = temp;
    } else {
      **str = '\0';
    }
  } else if (flag == '%') {
    **str = flag;
    (*str)++;
    length++;
  }

  return length;
}

void format_plus_and_minus(const char **format, int *zero_flag, int *plus) {
  while (**format == '+' || **format == '-') {
    if (**format == '-') {
      *zero_flag = -1;
      (*format)++;
    }
    if (**format == '+') {
      *plus = 1;
      (*format)++;
    }
  }
}

void format_numbers(const char **format, int *zero_flag, int *space) {
  if (**format == '0') {
    if (*zero_flag != -1) *zero_flag = 1;
    (*format)++;
  }
  while (**format >= '0' && **format <= '9') {
    *space = *space * 10 + (**format - '0');
    (*format)++;
  }
}

void format_dot(const char **format, int *precision, va_list args) {
  (*format)++;
  if (**format == '*') {
    *precision = va_arg(args, int);
    (*format)++;
  } else {
    *precision = 0;
    while (**format >= '0' && **format <= '9') {
      *precision = *precision * 10 + (**format - '0');
      (*format)++;
    }
  }
}

void format_h_l_L(const char **format, int *type) {
  if (**format == 'h') *type = 1;
  if (**format == 'l') *type = 2;
  if (**format == 'L') *type = 3;
  (*format)++;
}

void iterate_format_string(const char *format, va_list args, int *length,
                           char **str) {
  while (*format != '\0') {
    int precision = -1;
    int pref = 0;
    int type = 0;
    int space = 0;
    int zero_flag = 0;
    int plus = 0;
    if (*format != '%') {
      **str = *format;
      (*str)++;
      format++;
      (*length)++;
    } else {
      format++;
      if (*format == ' ') {
        plus = 2;
        format++;
      }
      if (*format == '+' || *format == '-') {
        format_plus_and_minus(&format, &zero_flag, &plus);
      }
      if (*format == '#') {
        pref = 1;
        format++;
      }
      if (*format == ' ') {
        plus = 2;
        format++;
      }
      if ((*format >= '0' && *format <= '9')) {
        format_numbers(&format, &zero_flag, &space);
      }
      if (*format == '*') {
        space = va_arg(args, int);
        format++;
      }
      if (*format == '+' || *format == '-') {
        format_plus_and_minus(&format, &zero_flag, &plus);
      }
      if (*format == '.') {
        format_dot(&format, &precision, args);
      }
      if (*format == 'h' || *format == 'l' || *format == 'L') {
        format_h_l_L(&format, &type);
      }
      *length += parser(*format, args, str, precision, pref, *length, space,
                        zero_flag, plus, type);
      if (*format != 'n') {
        while (**str != '\0') (*str)++;
      }
      format++;
    }
  }
  **str = '\0';
}

int s21_sprintf(char *str, const char *format, ...) {
  int length = 0;

  // s21_memset(str, 0, 100);

  va_list args;
  va_start(args, format);

  iterate_format_string(format, args, &length, &str);

  *str = '\0';
  va_end(args);
  return length;
}
