//
// Created by Peanut Grodd on 11/17/23.
//

#include "tests.h"

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "s21_toupper.h"

int main() {
  create_string(S21_NULL);
  int failed = 0;
  failed += s21_strncmp_test();
  failed += run_s21_strlen_tests();
  failed += s21_strerror_tests();
  failed += s21_memset_test();
  failed += run_to_upper_tests();
  failed += s21_strncpy_test();
  failed += s21_to_lower_test();
  failed += run_s21_strncat_tests();
  failed += s21_memcpy_test();
  failed += s21_insert_test();
  failed += s21_strrchr_test();
  failed += s21_strstr_test();
  failed += s21_strcspn_test();
  failed += s21_strchr_test();
  failed += s21_strtok_test();
  failed += s21_strpbrk_test();
  failed += s21_memcmp_tests();
  failed += s21_trim_tests();
  failed += s21_memchr_test();
  failed += s21_sprintf_test();

  return (failed == 0) ? 0 : CK_FAILURE;
}

int tests_run_within_valgrind() {
  char *p = getenv("LD_PRELOAD");
  if (p == NULL) return 0;
  return (strstr(p, "/valgrind/") != NULL || strstr(p, "/vgpreload") != NULL);
}

void generic_strncmp_test(char *a, char *b, size_t n, int res) {
  if (tests_run_within_valgrind())
    ck_assert_int_eq(strncmp(a, b, n), s21_strncmp(a, b, n));
  else
    ck_assert_int_eq(s21_strncmp(a, b, n), res);
}

START_TEST(strncmp_1) {
  char str_1[10] = "\0";
  char str_2[50] = "Per aspera ad astra";
  generic_strncmp_test(str_1, str_2, 0, 0);
}

END_TEST

START_TEST(strncmp_2) {
  char str_1[12] = "i am\nproger";
  char str_2[50] = " \n\0\0";
  generic_strncmp_test(str_1, str_2, 5, 1);
}

END_TEST

START_TEST(strncmp_3) {
  char str_1[10] = " a\n\0";
  char str_2[15] = "1234567890";
  generic_strncmp_test(str_1, str_2, 3, -1);
}

END_TEST

START_TEST(strncmp_4) {
  char str_1[15] = "1234975654";
  char str_2[15] = "1234567890";
  generic_strncmp_test(str_1, str_2, 1, 0);
}

END_TEST

START_TEST(strncmp_5) {
  char str_1[15] = "1234567890";
  char str_2[10] = "qwerty";
  generic_strncmp_test(str_1, str_2, 0, 0);
}

END_TEST

START_TEST(strncmp_6) {
  char s1[255] = "QWERTY";
  char s2[255] = "E";
  generic_strncmp_test(s1, s2, 6, 1);
}

END_TEST

START_TEST(S21_STRNCMP_NORMAL) { generic_strncmp_test("abc", "abc", 3, 0); }

END_TEST

START_TEST(S21_STRNCMP_EMPTY) { generic_strncmp_test("", "", 10, 0); }

END_TEST

START_TEST(S21_STRNCMP_WRONG_STRING) {
  char a[2] = {'a', -20};
  generic_strncmp_test(a, "abc", 10, 1);
}

START_TEST(S21_STRNCMP_WORNG_SIZE) {
  generic_strncmp_test("abc", "defg", 10, -1);
}

END_TEST

START_TEST(S21_STRNCMP_NORMAL_DIFF) {
  generic_strncmp_test("abc", "abd", 3, -1);
}

END_TEST
int s21_strncmp_test() {
  Suite *suite = suite_create("s21_strncmp");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, S21_STRNCMP_NORMAL);
  tcase_add_test(tc_core, S21_STRNCMP_WRONG_STRING);
  tcase_add_test(tc_core, S21_STRNCMP_EMPTY);
  tcase_add_test(tc_core, S21_STRNCMP_WORNG_SIZE);
  tcase_add_test(tc_core, S21_STRNCMP_NORMAL_DIFF);
  tcase_add_test(tc_core, strncmp_1);
  tcase_add_test(tc_core, strncmp_2);
  tcase_add_test(tc_core, strncmp_3);
  tcase_add_test(tc_core, strncmp_4);
  tcase_add_test(tc_core, strncmp_5);
  tcase_add_test(tc_core, strncmp_6);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return number_failed;
}

START_TEST(test_s21_strlen) {
  const char *str1 = "Hello, World!";
  const char *str2 = "";
  const char *str3 = NULL;

  ck_assert_int_eq(s21_strlen(str1), strlen(str1));
  ck_assert_int_eq(s21_strlen(str2), strlen(str2));
  ck_assert_int_eq(s21_strlen(str3), 0);
}

END_TEST

int run_s21_strlen_tests() {
  Suite *suite = suite_create("s21_string");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strlen);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_s21_strerror) {
  for (int i = 0; i < MAX_ERRNO + 1; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}

END_TEST

int s21_strerror_tests() {
  Suite *suite = suite_create("s21_strerror");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strerror);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

void generic_memset_test(const char *first, int ch, s21_size_t n) {
  char *s21 = create_string(first);
  char *orig = create_string(first);
  s21_memset(s21, ch, n);
  memset(orig, ch, n);
  ck_assert_str_eq(s21, orig);
  free(s21);
  free(orig);
}

START_TEST(S21_MEMSET_NORMAL) { generic_memset_test("aaa", 'b', 2); }

END_TEST

START_TEST(S21_MEMSET_ZERO) { generic_memset_test("aaa", 'b', 0); }

END_TEST

START_TEST(S21_MEMSET_CURSED_SYM) { generic_memset_test("aaa", -20, 2); }

END_TEST

int s21_memset_test() {
  Suite *suite = suite_create("s21_memset");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, S21_MEMSET_NORMAL);
  tcase_add_test(tc_core, S21_MEMSET_ZERO);
  tcase_add_test(tc_core, S21_MEMSET_CURSED_SYM);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_to_upper) {
  const char *str1 = "Hello, World!";
  const char *str2 = "";
  const char *str3 = S21_NULL;
  const char *str4 = "good job";
  const char *str5 = "empty";
  const char *str6 = "1+1*1";
  const char *str7 = " ";
  const char *str8 = "Testing 123";
  const char *str9 = "LowerCAse";
  const char *str10 = "UPPERCASE";
  const char *str11 = "MiXeD CaSe";
  const char *str12 = "!@#$%^&*()";
  const char *str13 = "12345";
  const char *str14 = "Special_Chars: \n\t\r";

  char *result1 = s21_to_upper(str1);
  char *result2 = s21_to_upper(str2);
  char *result3 = s21_to_upper(str3);
  char *result4 = s21_to_upper(str4);
  char *result5 = s21_to_upper(str5);
  char *result6 = s21_to_upper(str6);
  char *result7 = s21_to_upper(str7);
  char *result8 = s21_to_upper(str8);
  char *result9 = s21_to_upper(str9);
  char *result10 = s21_to_upper(str10);
  char *result11 = s21_to_upper(str11);
  char *result12 = s21_to_upper(str12);
  char *result13 = s21_to_upper(str13);
  char *result14 = s21_to_upper(str14);

  ck_assert_str_eq(result1, "HELLO, WORLD!");
  ck_assert_str_eq(result2, "");
  ck_assert_ptr_eq(result3, S21_NULL);
  ck_assert_str_eq(result4, "GOOD JOB");
  ck_assert_str_eq(result5, "EMPTY");
  ck_assert_str_eq(result6, "1+1*1");
  ck_assert_str_eq(result7, " ");
  ck_assert_str_eq(result8, "TESTING 123");
  ck_assert_str_eq(result9, "LOWERCASE");
  ck_assert_str_eq(result10, "UPPERCASE");
  ck_assert_str_eq(result11, "MIXED CASE");
  ck_assert_str_eq(result12, "!@#$%^&*()");
  ck_assert_str_eq(result13, "12345");
  ck_assert_str_eq(result14, "SPECIAL_CHARS: \n\t\r");

  // Освобождаю выделенную память
  free(result1);
  free(result2);
  // result3 не освобождаем - она равна S21_NULL
  free(result4);
  free(result5);
  free(result6);
  free(result7);
  free(result8);
  free(result9);
  free(result10);
  free(result11);
  free(result12);
  free(result13);
  free(result14);
}

END_TEST

int run_to_upper_tests() {
  Suite *suite = suite_create("s21_to_upper");
  TCase *tc_core = tcase_create("Core");

  // Тестовый случай
  tcase_add_test(tc_core, test_to_upper);
  suite_add_tcase(suite, tc_core);

  // Исполнитель тестов и запуск тестов
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_s21_strncat) {
  char dest1[50] = "Hello, ";
  const char *src1 = "World!";
  char dest2[50] = "";
  const char *src2 = "Test";
  char dest3[50] = "123";
  const char *src3 = "456";
  char dest4[50] = "abc";
  const char *src4 = "";
  char dest5[50] = "";
  const char *src5 = "123";
  char dest6[50] = "abcdef";
  const char *src6 = "ghijkl";
  char dest7[50] = "abcd";
  const char *src7 = "efgh";
  char dest8[50] = "0123456789";
  const char *src8 = "abcdef";
  char dest9[50] = "short";
  const char *src9 = "longerstring";
  char dest10[50] = "123";
  const char *src10 = "4567890";

  s21_strncat(dest1, src1, 10);
  s21_strncat(dest2, src2, 5);
  s21_strncat(dest3, src3, 2);
  s21_strncat(dest4, src4, 10);
  s21_strncat(dest5, src5, 0);
  s21_strncat(dest6, src6, 3);
  s21_strncat(dest7, src7, 0);
  s21_strncat(dest8, src8, 5);
  s21_strncat(dest9, src9, 5);
  s21_strncat(dest10, src10, 7);

  ck_assert_str_eq(dest1, "Hello, World!");
  ck_assert_str_eq(dest2, "Test");
  ck_assert_str_eq(dest3, "12345");
  ck_assert_str_eq(dest4, "abc");
  ck_assert_str_eq(dest5, "");
  ck_assert_str_eq(dest6, "abcdefghi");
  ck_assert_str_eq(dest7, "abcd");
  ck_assert_str_eq(dest8, "0123456789abcde");
  ck_assert_str_eq(dest9, "shortlonge");
  ck_assert_str_eq(dest10, "1234567890");
}

END_TEST

int run_s21_strncat_tests() {
  Suite *suite = suite_create("s21_strncat");
  TCase *tc_core = tcase_create("Core");

  // Тестовый случай
  tcase_add_test(tc_core, test_s21_strncat);
  suite_add_tcase(suite, tc_core);

  // Исполнитель тестов и запуск тестов
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

void generic_strncpy_test(const char *what, s21_size_t first_size,
                          s21_size_t n) {
  char *orig = calloc(first_size, sizeof(char));
  char *s21 = calloc(first_size, sizeof(char));
  ck_assert_str_eq(s21_strncpy(s21, what, n), strncpy(orig, what, n));
  free(s21);
  free(orig);
}

START_TEST(S21_STRNCPY_NORMAL) { generic_strncpy_test("sample text", 13, 12); }

END_TEST

START_TEST(S21_STRNCPY_EMPTY) { generic_strncpy_test("sample text", 0, 12); }

END_TEST

START_TEST(S21_STRNCPY_NORMAL_LESS) {
  generic_strncpy_test("sample text", 13, 2);
}

END_TEST

START_TEST(S21_STRNCPY_NORMAL_MORE) {
  generic_strncpy_test("sample text", 13, 15);
}

START_TEST(strncpy_1) {
  char str_1[50] = "qwerty";
  char str_2[50] = "12345";
  ck_assert_str_eq(strncpy(str_1, str_2, 2 * 2),
                   s21_strncpy(str_1, str_2, 2 * 2));
}

END_TEST

START_TEST(strncpy_2) {
  char str_1[60] = "50 pdfh";
  char str_2[50] = "HELLO!\n5";
  ck_assert_str_eq(strncpy(str_1, str_2, 5 - 1),
                   s21_strncpy(str_1, str_2, 5 - 1));
}

END_TEST

// START_TEST(strncpy_3) {
//   char str_1[50] = "\n/f\f/n";
//   char str_2[50] = "55/f/n\f\n";
//   ck_assert_str_eq(strncpy(str_1, str_2, 0), s21_strncpy(str_1, str_2, 0));
// } END_TEST

START_TEST(strncpy_4) {
  char str_1[20] = "abracadabra ";
  char str_2[] = " 123";
  ck_assert_str_eq(strncpy(str_1, str_2, 10), s21_strncpy(str_1, str_2, 10));
}

END_TEST

START_TEST(strncpy_5) {
  char str_1[50] = "s_=+*&/te   ";
  char str_2[50] = "vosem'";
  ck_assert_str_eq(strncpy(str_1, str_2, 6), s21_strncpy(str_1, str_2, 6));
}

END_TEST

START_TEST(strncpy_6) {
  char str_1[50] = "8h2f7y10/n5 6";
  char str_2[] = "'40' -l";
  ck_assert_str_eq(strncpy(str_1, str_2, 10 / 5),
                   s21_strncpy(str_1, str_2, 10 / 5));
}

END_TEST

START_TEST(strncpy_7) {
  char s1[255] = "QWERTY";
  char s2[255] = "Eeee";
  ck_assert_pstr_eq(strncpy(s1, s2, 2), s21_strncpy(s1, s2, 2));
}

END_TEST

END_TEST

int s21_strncpy_test() {
  Suite *suite = suite_create("s21_strncpy");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, S21_STRNCPY_NORMAL);
  tcase_add_test(tc_core, S21_STRNCPY_EMPTY);
  tcase_add_test(tc_core, S21_STRNCPY_NORMAL_LESS);
  tcase_add_test(tc_core, S21_STRNCPY_NORMAL_MORE);
  tcase_add_test(tc_core, strncpy_1);
  tcase_add_test(tc_core, strncpy_2);
  //  tcase_add_test(tc_core, strncpy_3);
  tcase_add_test(tc_core, strncpy_4);
  tcase_add_test(tc_core, strncpy_5);
  tcase_add_test(tc_core, strncpy_6);
  tcase_add_test(tc_core, strncpy_7);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(strchr_1) {
  char str[] = "Hello world";
  int c = 3;
  ck_assert_ptr_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_2) {
  char str[] = "Hello world\0";
  int c = 5;
  ck_assert_ptr_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_3) {
  char str[] = "Hello world";
  int c = 12;
  ck_assert_ptr_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_4) {
  char str[] = "Hello world";
  int c = '\0';
  ck_assert_str_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_5) {
  char str[] = "";
  int c = 0;
  ck_assert_ptr_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_6) {
  char str[] = "a\n\0";
  int c = 'a';
  ck_assert_str_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_7) {
  char str[] = "aaaaaaaaaaaaaaaaaaa";
  int c = 'b';
  ck_assert_ptr_eq(s21_strchr(str, c), strchr(str, c));
}

END_TEST

START_TEST(strchr_8) {
  char *s1 = "QWERTY";
  char s2 = 'E';
  ck_assert_pstr_eq(strchr(s1, s2), s21_strchr(s1, s2));
}

END_TEST

START_TEST(test_s21_strchr) {
  const char str1[] = "https://edu.21-school.ru/project/26477/task";
  const char ch1 = '.';
  const char str2[] = "Hello, World!";
  const char ch2 = ',';
  const char str3[] = "School 21";
  const char ch3 = '.';
  const char str4[] = "School 21";
  const char ch4 = 'S';
  const char str5[] = "School ";
  const char ch5 = ' ';

  ck_assert_str_eq(s21_strchr(str1, ch1), strchr(str1, ch1));
  ck_assert_str_eq(s21_strchr(str2, ch2), strchr(str2, ch2));
  ck_assert_ptr_eq(s21_strchr(str3, ch3), strchr(str3, ch3));
  ck_assert_str_eq(s21_strchr(str4, ch4), strchr(str4, ch4));
  ck_assert_str_eq(s21_strchr(str5, ch5), strchr(str5, ch5));
}

END_TEST

int s21_strchr_test() {
  Suite *suite = suite_create("s21_strchr");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strchr);
  tcase_add_test(tc_core, strchr_1);
  tcase_add_test(tc_core, strchr_2);
  tcase_add_test(tc_core, strchr_3);
  tcase_add_test(tc_core, strchr_4);
  tcase_add_test(tc_core, strchr_5);
  tcase_add_test(tc_core, strchr_6);
  tcase_add_test(tc_core, strchr_7);
  tcase_add_test(tc_core, strchr_8);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_s21_to_lower) {
  char *str1 = "Hello, World!";
  char *str2 = "";
  char *str3 = S21_NULL;
  char *str4 = "SCHOOL 21";
  char *str5 = "empty";
  char *str6 = " ";

  char *result1 = s21_to_lower(str1);
  char *result2 = s21_to_lower(str2);
  char *result3 = s21_to_lower(str3);
  char *result4 = s21_to_lower(str4);
  char *result5 = s21_to_lower(str5);
  char *result6 = s21_to_lower(str6);

  ck_assert_str_eq(result1, "hello, world!");
  ck_assert_str_eq(result2, "");
  ck_assert_ptr_eq(result3, S21_NULL);
  ck_assert_str_eq(result4, "school 21");
  ck_assert_str_eq(result5, "empty");
  ck_assert_str_eq(result6, " ");

  // Освобождаем память после использования
  free(result1);
  free(result2);
  free(result3);
  free(result4);
  free(result5);
  free(result6);
}

END_TEST

int s21_to_lower_test() {
  Suite *suite = suite_create("s21_to_lower");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_to_lower);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

void generic_memcpy_test(const char *what, s21_size_t size, s21_size_t n) {
  char *orig = calloc(size, sizeof(char));
  char *s21 = calloc(size, sizeof(char));

  ck_assert_str_eq(memcpy(orig, what, n), s21_memcpy(s21, what, n));
  free(orig);
  free(s21);
}

START_TEST(S21_MEMCPY_NORMAL) { generic_memcpy_test("subdvach", 10, 8); }

END_TEST

START_TEST(S21_MEMCPY_EMPTY) { generic_memcpy_test("subdvach", 0, 8); }

END_TEST

START_TEST(S21_MEMCPY_NORMAL_LESS) { generic_memcpy_test("subdvach", 10, 3); }

END_TEST

int s21_memcpy_test() {
  Suite *suite = suite_create("s21_memcpy");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, S21_MEMCPY_NORMAL);
  tcase_add_test(tc_core, S21_MEMCPY_EMPTY);
  tcase_add_test(tc_core, S21_MEMCPY_NORMAL_LESS);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

void generic_strstr_test(const char *haystack, const char *needle) {
  ck_assert_ptr_eq(strstr(haystack, needle), s21_strstr(haystack, needle));
}

START_TEST(S21_STRSTR_NORMAL) { generic_strstr_test("appletree", "tree"); }

END_TEST

START_TEST(S21_STRSTR_EMPTY) { generic_strstr_test("", "tree"); }

END_TEST

START_TEST(S21_STRSTR_ONE) { generic_strstr_test("appletree", "t"); }

END_TEST

START_TEST(S21_STRSTR_WRONG_SEEK) { generic_strstr_test("appletree", ""); }

END_TEST

START_TEST(S21_STRSTR_NOT_FULL) {
  generic_strstr_test("_xXxStart_xXx", "_xXx_");
}

END_TEST

int s21_strstr_test() {
  Suite *suite = suite_create("s21_strstr");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, S21_STRSTR_NORMAL);
  tcase_add_test(tc_core, S21_STRSTR_EMPTY);
  tcase_add_test(tc_core, S21_STRSTR_ONE);
  tcase_add_test(tc_core, S21_STRSTR_WRONG_SEEK);
  tcase_add_test(tc_core, S21_STRSTR_NOT_FULL);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_s21_strcspn) {
  const char *str1_1 = "Hello, World!";
  const char *str2_1 = " ,!";
  size_t result1 = s21_strcspn(str1_1, str2_1);
  ck_assert_int_eq(result1, 5);

  const char *str1_2 = "";
  const char *str2_2 = "abc";
  size_t result2 = s21_strcspn(str1_2, str2_2);
  ck_assert_int_eq(result2, 0);

  const char *str1_3 = "Hello, World!";
  const char *str2_3 = "";
  size_t result3 = s21_strcspn(str1_3, str2_3);
  ck_assert_int_eq(result3, 13);

  const char *str1_4 = "";
  const char *str2_4 = "";
  size_t result4 = s21_strcspn(str1_4, str2_4);
  ck_assert_int_eq(result4, 0);

  const char *str1_5 = "Hello, World!";
  const char *str2_5 = "123";
  size_t result5 = s21_strcspn(str1_5, str2_5);
  ck_assert_int_eq(result5, 13);

  const char *str1_6 = NULL;
  const char *str2_6 = "abc";
  size_t result6 = s21_strcspn(str1_6, str2_6);
  ck_assert_int_eq(result6, 0);

  const char *str1_7 = "Hello, World!";
  const char *str2_7 = NULL;
  size_t result7 = s21_strcspn(str1_7, str2_7);
  ck_assert_int_eq(result7, 0);

  const char *str1_8 = NULL;
  const char *str2_8 = NULL;
  size_t result8 = s21_strcspn(str1_8, str2_8);
  ck_assert_int_eq(result8, 0);

  const char *str1_9 = "Hello, World!";
  const char *str2_9 = "abc";
  size_t result9 = s21_strcspn(str1_9, str2_9);
  ck_assert_int_eq(result9, 13);

  const char *str1_10 = "Hi";
  const char *str2_10 = "World!";
  size_t result10 = s21_strcspn(str1_10, str2_10);
  ck_assert_int_eq(result10, 2);

  const char *str1_11 = "aaaaaa";
  const char *str2_11 = "a";
  size_t result11 = s21_strcspn(str1_11, str2_11);
  ck_assert_int_eq(result11, 0);

  const char *str1_12 = "aaaaaa";
  const char *str2_12 = "b";
  size_t result12 = s21_strcspn(str1_12, str2_12);
  ck_assert_int_eq(result12, 6);

  const char *str1_13 = "!@#$%^&*()";
  const char *str2_13 = "^";
  size_t result13 = s21_strcspn(str1_13, str2_13);
  ck_assert_int_eq(result13, 5);

  const char *str1_14 = "你好，世界！";
  const char *str2_14 = "，";
  size_t result14 = s21_strcspn(str1_14, str2_14);
  ck_assert_int_eq(result14, 6);

  const char *str1_15 = "你好，世界！";
  const char *str2_15 = "!";
  size_t result15 = s21_strcspn(str1_15, str2_15);
  ck_assert_int_eq(result15, 18);
}

END_TEST

int s21_strcspn_test() {
  Suite *suite = suite_create("s21_strcspn");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strcspn);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(strrchr_1) {
  char str[] = "Techie Delight – Ace the Technical Interviews";
  int c = 'D';
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_2) {
  char str[] = "Hello World";
  int c = 'e';
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_3) {
  char str[] = "Hello World";
  int c = 'a';
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_4) {
  char str[] = "Hello World";
  int c = 0;
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_5) {
  char str[] = "";
  int c = 'a';
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_6) {
  char str[] = "Privet verter";
  int c = '\0';
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_7) {
  char str[] = "a\n\0";
  int c = '\0';
  ck_assert_ptr_eq(strrchr(str, c), s21_strrchr(str, c));
}

END_TEST

START_TEST(strrchr_8) {
  char *s1 = "QWERTY";
  char s2 = 'E';
  ck_assert_pstr_eq(strrchr(s1, s2), s21_strrchr(s1, s2));
}

END_TEST

START_TEST(test_s21_strrchr) {
  const char str1[] = "https://edu.21-school.ru/project/26477/task";
  const char ch1 = '.';
  const char str2[] = "Hello, World!";
  const char ch2 = ',';
  const char str3[] = "School 21";
  const char ch3 = '.';
  const char str4[] = "School 21";
  const char ch4 = 'S';
  const char str5[] = "School ";
  const char ch5 = ' ';

  ck_assert_str_eq(s21_strrchr(str1, ch1), strrchr(str1, ch1));
  ck_assert_str_eq(s21_strrchr(str2, ch2), strrchr(str2, ch2));
  ck_assert_ptr_eq(s21_strrchr(str3, ch3), strrchr(str3, ch3));
  ck_assert_str_eq(s21_strrchr(str4, ch4), strrchr(str4, ch4));
  ck_assert_str_eq(s21_strrchr(str5, ch5), strrchr(str5, ch5));
}

END_TEST

int s21_strrchr_test() {
  Suite *suite = suite_create("s21_strrchr");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strrchr);
  tcase_add_test(tc_core, strrchr_1);
  tcase_add_test(tc_core, strrchr_2);
  tcase_add_test(tc_core, strrchr_3);
  tcase_add_test(tc_core, strrchr_4);
  tcase_add_test(tc_core, strrchr_5);
  tcase_add_test(tc_core, strrchr_6);
  tcase_add_test(tc_core, strrchr_7);
  tcase_add_test(tc_core, strrchr_8);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(s21_sprintf_f_test) {
  char str[80];
  char str_orig[80];

  double b = 1;
  int res_1 = s21_sprintf(str, "%.f", b);
  int res_1_orig = sprintf(str_orig, "%.f", b);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  double b_2 = 0.00000002345;
  int res_2 = s21_sprintf(str, "%+-10.3f", b_2);
  int res_2_orig = sprintf(str_orig, "%+-10.3f", b_2);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  long double b_3 = -23.087654;
  int res_3 = s21_sprintf(str, "%20.10Lf", b_3);
  int res_3_orig = sprintf(str_orig, "%20.10Lf", b_3);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  //    double b_4 = 0;
  //    int res_4 = s21_sprintf(str, "%0+.15F", b_4);
  //    int res_4_orig = sprintf(str_orig, "%0+.15F", b_4);
  //
  //    ck_assert_int_eq(res_4, res_4_orig);
  //    ck_assert_str_eq(str, str_orig);

  long double b_5 = 0;
  int res_5 = s21_sprintf(str, "%#.0Lf", b_5);
  int res_5_orig = sprintf(str_orig, "%#.0Lf", b_5);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  //    double b_6 = 1.0 / 0.0;
  //
  //    int res_6 = s21_sprintf(str, "%#10.0f", b_6);
  //    int res_6_orig = sprintf(str_orig, "%#10.0f", b_6);

  //    ck_assert_int_eq(res_6, res_6_orig);
  //    ck_assert_str_eq(str, str_orig);

  double b_7 = 1234523459.123456789;
  int res_7 = s21_sprintf(str, "%.5f", b_7);
  int res_7_orig = sprintf(str_orig, "%.5f", b_7);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  double b_8 = 1;
  int res_8 = s21_sprintf(str, "%f", b_8);
  int res_8_orig = sprintf(str_orig, "%f", b_8);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  double b_9 = 0.00345;
  int res_9 = s21_sprintf(str, "%f", b_9);
  int res_9_orig = sprintf(str_orig, "%f", b_9);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  double b_10 = 0.00345;
  int res_10 = s21_sprintf(str, "%f", b_10);
  int res_10_orig = sprintf(str_orig, "%f", b_10);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  double b_11 = 1234567.0034598765432987654321;
  int res_11 = s21_sprintf(str, "%20f", b_11);
  int res_11_orig = sprintf(str_orig, "%20f", b_11);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_n_test) {
  char str[80];
  char str_orig[80];

  int c_1 = 0, c_1_orig = 0;
  int res_1 = s21_sprintf(str, "%n", &c_1);
  int res_1_orig = sprintf(str_orig, "%n", &c_1_orig);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_int_eq(c_1, c_1_orig);
  ck_assert_str_eq(str, str_orig);

  int c_2, c_2_orig;
  int res_2 = s21_sprintf(str, "12345%n678910", &c_2);
  int res_2_orig = sprintf(str_orig, "12345%n678910", &c_2_orig);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_int_eq(c_2, c_2_orig);
  ck_assert_str_eq(str, str_orig);

  int c_3, c_3_orig;
  int res_3 = s21_sprintf(str, "%nqwerty", &c_3);
  int res_3_orig = sprintf(str_orig, "%nqwerty", &c_3_orig);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_int_eq(c_3, c_3_orig);
  ck_assert_str_eq(str, str_orig);

  int c_4, c_4_orig;
  int res_4 = s21_sprintf(str, "hhhhh%n%%", &c_4);
  int res_4_orig = sprintf(str_orig, "hhhhh%n%%", &c_4_orig);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_int_eq(c_4, c_4_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_x_X_test) {
  int arr1[] = {0, -1, 1, 123, 1234, -1383, 9387};
  char str[80];
  char str1[80];
  int res = s21_sprintf(str, "%x", arr1[0]);
  int res1 = sprintf(str1, "%x", arr1[0]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%#10.7hx", (unsigned short)arr1[3]);
  res1 = sprintf(str1, "%#10.7hx", (unsigned short)arr1[3]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%-#10.3lx", (unsigned long)arr1[3]);
  res1 = sprintf(str1, "%-#10.3lx", (unsigned long)arr1[3]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%X", arr1[0]);
  res1 = sprintf(str1, "%X", arr1[0]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%#10.7lX", (unsigned long)arr1[3]);
  res1 = sprintf(str1, "%#10.7lX", (unsigned long)arr1[3]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%-#10.3hX", (unsigned short)arr1[3]);
  res1 = sprintf(str1, "%-#10.3hX", (unsigned short)arr1[3]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%-#*.3hX", 10, (unsigned short)arr1[3]);
  res1 = sprintf(str1, "%-#*.3hX", 10, (unsigned short)arr1[3]);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);
}

END_TEST

START_TEST(s21_sprintf_c_test) {
  char str[80];
  char str1[80];
  char x = 'c';
  int res = s21_sprintf(str, "%-10c", x);
  int res1 = sprintf(str1, "%-10c", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%10c", x);
  res1 = sprintf(str1, "%10c", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);
}

END_TEST

START_TEST(s21_sprintf_s_test) {
  char str[80];
  char str1[80];
  char *x = "blablabla";
  int res = s21_sprintf(str, "%-11.10s", x);
  int res1 = sprintf(str1, "%-11.10s", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%11.8s", x);
  res1 = sprintf(str1, "%11.8s", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);
}

END_TEST

START_TEST(s21_sprintf_u_test) {
  char str[80];
  char str_orig[80];
  unsigned int a = 2345;

  int res_1 = s21_sprintf(str, "%10.5u", a);
  int res_1_orig = sprintf(str_orig, "%10.5u", a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_2 = s21_sprintf(str, "%10.5u", a);
  int res_2_orig = sprintf(str_orig, "%10.5u", a);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%3.5u", a);
  int res_3_orig = sprintf(str_orig, "%3.5u", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%0u", a);
  int res_4_orig = sprintf(str_orig, "%0u", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%-u", a);
  int res_5_orig = sprintf(str_orig, "%-u", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%u", a);
  int res_6_orig = sprintf(str_orig, "%u", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%5u", a);
  int res_7_orig = sprintf(str_orig, "%5u", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_8 = s21_sprintf(str, "%05u", a);
  int res_8_orig = sprintf(str_orig, "%05u", a);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%-5u", a);
  int res_9_orig = sprintf(str_orig, "%-5u", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_10 = s21_sprintf(str, "%.5u", a);
  int res_10_orig = sprintf(str_orig, "%.5u", a);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%.5u", a);
  int res_11_orig = sprintf(str_orig, "%.5u", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%-.5u", a);
  int res_12_orig = sprintf(str_orig, "%-.5u", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_13 = s21_sprintf(str, "%-10.5u", a);
  int res_13_orig = sprintf(str_orig, "%-10.5u", a);

  ck_assert_int_eq(res_13, res_13_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%3.5u", a);
  int res_14_orig = sprintf(str_orig, "%3.5u", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%-3.5u", a);
  int res_15_orig = sprintf(str_orig, "%-3.5u", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "%6.2u", a);
  int res_16_orig = sprintf(str_orig, "%6.2u", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_17 = s21_sprintf(str, "%3.2u", a);
  int res_17_orig = sprintf(str_orig, "%3.2u", a);

  ck_assert_int_eq(res_17, res_17_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%01u", a);
  int res_18_orig = sprintf(str_orig, "%01u", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_19 = s21_sprintf(str, "%hu", (unsigned short)a);
  int res_19_orig = sprintf(str_orig, "%hu", (unsigned short)a);

  ck_assert_int_eq(res_19, res_19_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%lu", (unsigned long)a);
  int res_20_orig = sprintf(str_orig, "%lu", (unsigned long)a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test) {
  char str[80];
  char str_orig[80];
  int a = 2345;

  int res_1 = s21_sprintf(str, "%10.5d", a);
  int res_1_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_2 = s21_sprintf(str, "% 10.5d", a);
  int res_2_orig = sprintf(str_orig, "% 10.5d", a);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%3.5d", a);
  int res_3_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%d", a);
  int res_4_orig = sprintf(str_orig, "%d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%0d", a);
  int res_5_orig = sprintf(str_orig, "%0d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%-d", a);
  int res_6_orig = sprintf(str_orig, "%-d", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_8 = s21_sprintf(str, "% d", a);
  int res_8_orig = sprintf(str_orig, "% d", a);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%5d", a);
  int res_9_orig = sprintf(str_orig, "%5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_10 = s21_sprintf(str, "%05d", a);
  int res_10_orig = sprintf(str_orig, "%05d", a);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%-5d", a);
  int res_11_orig = sprintf(str_orig, "%-5d", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_13 = s21_sprintf(str, "% 5d", a);
  int res_13_orig = sprintf(str_orig, "% 5d", a);

  ck_assert_int_eq(res_13, res_13_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%.5d", a);
  int res_14_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%05d", a);
  int res_15_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "% .5d", a);
  int res_16_orig = sprintf(str_orig, "% .5d", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_17 = s21_sprintf(str, "%+.5d", a);
  int res_17_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_17, res_17_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%-.5d", a);
  int res_18_orig = sprintf(str_orig, "%-.5d", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_19 = s21_sprintf(str, "%10.5d", a);
  int res_19_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_19, res_19_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_21 = s21_sprintf(str, "%-10.5d", a);
  int res_21_orig = sprintf(str_orig, "%-10.5d", a);

  ck_assert_int_eq(res_21, res_21_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%3.5d", a);
  int res_22_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_23 = s21_sprintf(str, "% 3.5d", a);
  int res_23_orig = sprintf(str_orig, "% 3.5d", a);

  ck_assert_int_eq(res_23, res_23_orig);
  ck_assert_str_eq(str, str_orig);

  int res_24 = s21_sprintf(str, "%+3.5d", a);
  int res_24_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_24, res_24_orig);
  ck_assert_str_eq(str, str_orig);

  int res_25 = s21_sprintf(str, "%-3.5d", a);
  int res_25_orig = sprintf(str_orig, "%-3.5d", a);

  ck_assert_int_eq(res_25, res_25_orig);
  ck_assert_str_eq(str, str_orig);

  int res_26 = s21_sprintf(str, "%hd", (unsigned short)a);
  int res_26_orig = sprintf(str_orig, "%hd", (unsigned short)a);

  ck_assert_int_eq(res_26, res_26_orig);
  ck_assert_str_eq(str, str_orig);

  int res_27 = s21_sprintf(str, "%ld", (unsigned long)a);
  int res_27_orig = sprintf(str_orig, "%ld", (unsigned long)a);

  ck_assert_int_eq(res_27, res_27_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test_2) {
  char str[80];
  char str_orig[80];
  int a = -23454546;

  int res_1 = s21_sprintf(str, "%10.*d", 5, a);
  int res_1_orig = sprintf(str_orig, "%10.*d", 5, a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_2 = s21_sprintf(str, "% 10.5d", a);
  int res_2_orig = sprintf(str_orig, "% 10.5d", a);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%3.5d", a);
  int res_3_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%d", a);
  int res_4_orig = sprintf(str_orig, "%d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%0d", a);
  int res_5_orig = sprintf(str_orig, "%0d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%-d", a);
  int res_6_orig = sprintf(str_orig, "%-d", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_8 = s21_sprintf(str, "% d", a);
  int res_8_orig = sprintf(str_orig, "% d", a);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%5d", a);
  int res_9_orig = sprintf(str_orig, "%5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_10 = s21_sprintf(str, "%0*d", 5, a);
  int res_10_orig = sprintf(str_orig, "%0*d", 5, a);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%-5d", a);
  int res_11_orig = sprintf(str_orig, "%-5d", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_13 = s21_sprintf(str, "% 5d", a);
  int res_13_orig = sprintf(str_orig, "% 5d", a);

  ck_assert_int_eq(res_13, res_13_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%.5d", a);
  int res_14_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%.5d", a);
  int res_15_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "% .5d", a);
  int res_16_orig = sprintf(str_orig, "% .5d", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_17 = s21_sprintf(str, "%+.5d", a);
  int res_17_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_17, res_17_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%-.5d", a);
  int res_18_orig = sprintf(str_orig, "%-.5d", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_19 = s21_sprintf(str, "%10.5d", a);
  int res_19_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_19, res_19_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_21 = s21_sprintf(str, "%-10.5d", a);
  int res_21_orig = sprintf(str_orig, "%-10.5d", a);

  ck_assert_int_eq(res_21, res_21_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%3.5d", a);
  int res_22_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_23 = s21_sprintf(str, "% 3.5d", a);
  int res_23_orig = sprintf(str_orig, "% 3.5d", a);

  ck_assert_int_eq(res_23, res_23_orig);
  ck_assert_str_eq(str, str_orig);

  int res_24 = s21_sprintf(str, "%+3.5d", a);
  int res_24_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_24, res_24_orig);
  ck_assert_str_eq(str, str_orig);

  int res_25 = s21_sprintf(str, "%-3.5d", a);
  int res_25_orig = sprintf(str_orig, "%-3.5d", a);

  ck_assert_int_eq(res_25, res_25_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test_3) {
  char str[80];
  char str_orig[80];
  int a = 234;

  int res_1 = s21_sprintf(str, "%10.5d", a);
  int res_1_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_2 = s21_sprintf(str, "% 10.5d", a);
  int res_2_orig = sprintf(str_orig, "% 10.5d", a);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%3.5d", a);
  int res_3_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%d", a);
  int res_4_orig = sprintf(str_orig, "%d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%0d", a);
  int res_5_orig = sprintf(str_orig, "%0d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%-d", a);
  int res_6_orig = sprintf(str_orig, "%-d", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_8 = s21_sprintf(str, "% d", a);
  int res_8_orig = sprintf(str_orig, "% d", a);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%5d", a);
  int res_9_orig = sprintf(str_orig, "%5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_10 = s21_sprintf(str, "%05d", a);
  int res_10_orig = sprintf(str_orig, "%05d", a);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%-5d", a);
  int res_11_orig = sprintf(str_orig, "%-5d", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_13 = s21_sprintf(str, "% 5d", a);
  int res_13_orig = sprintf(str_orig, "% 5d", a);

  ck_assert_int_eq(res_13, res_13_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%.5d", a);
  int res_14_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%.5d", a);
  int res_15_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "% .5d", a);
  int res_16_orig = sprintf(str_orig, "% .5d", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_17 = s21_sprintf(str, "%+.5d", a);
  int res_17_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_17, res_17_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%-.5d", a);
  int res_18_orig = sprintf(str_orig, "%-.5d", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_19 = s21_sprintf(str, "%10.5d", a);
  int res_19_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_19, res_19_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_21 = s21_sprintf(str, "%-10.5d", a);
  int res_21_orig = sprintf(str_orig, "%-10.5d", a);

  ck_assert_int_eq(res_21, res_21_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%3.5d", a);
  int res_22_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_23 = s21_sprintf(str, "% 3.5d", a);
  int res_23_orig = sprintf(str_orig, "% 3.5d", a);

  ck_assert_int_eq(res_23, res_23_orig);
  ck_assert_str_eq(str, str_orig);

  int res_24 = s21_sprintf(str, "%+3.5d", a);
  int res_24_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_24, res_24_orig);
  ck_assert_str_eq(str, str_orig);

  int res_25 = s21_sprintf(str, "%-3.5d", a);
  int res_25_orig = sprintf(str_orig, "%-3.5d", a);

  ck_assert_int_eq(res_25, res_25_orig);
  ck_assert_str_eq(str, str_orig);

  int res_26 = s21_sprintf(str, "%+6.2d", a);
  int res_26_orig = sprintf(str_orig, "%+6.2d", a);

  ck_assert_int_eq(res_26, res_26_orig);
  ck_assert_str_eq(str, str_orig);

  int res_27 = s21_sprintf(str, "%+6.3d", a);
  int res_27_orig = sprintf(str_orig, "%+6.3d", a);

  ck_assert_int_eq(res_27, res_27_orig);
  ck_assert_str_eq(str, str_orig);

  int res_28 = s21_sprintf(str, "%+6.4d", a);
  int res_28_orig = sprintf(str_orig, "%+6.4d", a);

  ck_assert_int_eq(res_28, res_28_orig);
  ck_assert_str_eq(str, str_orig);

  int res_29 = s21_sprintf(str, "%-6.2d", -a);
  int res_29_orig = sprintf(str_orig, "%-6.2d", -a);

  ck_assert_int_eq(res_29, res_29_orig);
  ck_assert_str_eq(str, str_orig);

  int res_30 = s21_sprintf(str, "%+05d", a);
  int res_30_orig = sprintf(str_orig, "%+05d", a);

  ck_assert_int_eq(res_30, res_30_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test_4) {
  char str[80];
  char str_orig[80];
  int a = -23;

  int res_1 = s21_sprintf(str, "%10.5d", a);
  int res_1_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_2 = s21_sprintf(str, "% 10.5d", a);
  int res_2_orig = sprintf(str_orig, "% 10.5d", a);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%3.5d", a);
  int res_3_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%d", a);
  int res_4_orig = sprintf(str_orig, "%d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%0d", a);
  int res_5_orig = sprintf(str_orig, "%0d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%-d", a);
  int res_6_orig = sprintf(str_orig, "%-d", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_8 = s21_sprintf(str, "% d", a);
  int res_8_orig = sprintf(str_orig, "% d", a);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%5d", a);
  int res_9_orig = sprintf(str_orig, "%5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_10 = s21_sprintf(str, "%05d", a);
  int res_10_orig = sprintf(str_orig, "%05d", a);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%-5d", a);
  int res_11_orig = sprintf(str_orig, "%-5d", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_13 = s21_sprintf(str, "% 5d", a);
  int res_13_orig = sprintf(str_orig, "% 5d", a);

  ck_assert_int_eq(res_13, res_13_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%.5d", a);
  int res_14_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%.5d", a);
  int res_15_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "% .5d", a);
  int res_16_orig = sprintf(str_orig, "% .5d", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_17 = s21_sprintf(str, "%+.5d", a);
  int res_17_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_17, res_17_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%-.5d", a);
  int res_18_orig = sprintf(str_orig, "%-.5d", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_19 = s21_sprintf(str, "%10.5d", a);
  int res_19_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_19, res_19_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_21 = s21_sprintf(str, "%-10.5d", a);
  int res_21_orig = sprintf(str_orig, "%-10.5d", a);

  ck_assert_int_eq(res_21, res_21_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%3.5d", a);
  int res_22_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_23 = s21_sprintf(str, "% 3.5d", a);
  int res_23_orig = sprintf(str_orig, "% 3.5d", a);

  ck_assert_int_eq(res_23, res_23_orig);
  ck_assert_str_eq(str, str_orig);

  int res_24 = s21_sprintf(str, "%+3.5d", a);
  int res_24_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_24, res_24_orig);
  ck_assert_str_eq(str, str_orig);

  int res_25 = s21_sprintf(str, "%-3.5d", a);
  int res_25_orig = sprintf(str_orig, "%-3.5d", a);

  ck_assert_int_eq(res_25, res_25_orig);
  ck_assert_str_eq(str, str_orig);

  int res_26 = s21_sprintf(str, "%+05d", a);
  int res_26_orig = sprintf(str_orig, "%+05d", a);

  ck_assert_int_eq(res_26, res_26_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test_5) {
  char str[80];
  char str_orig[80];
  int a = -2344;

  int res_2 = s21_sprintf(str, "% 10.5d", a);
  int res_2_orig = sprintf(str_orig, "% 10.5d", a);

  ck_assert_int_eq(res_2, res_2_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%3.5d", a);
  int res_3_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%d", a);
  int res_4_orig = sprintf(str_orig, "%d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%0d", a);
  int res_5_orig = sprintf(str_orig, "%0d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%-d", a);
  int res_6_orig = sprintf(str_orig, "%-d", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_8 = s21_sprintf(str, "% d", a);
  int res_8_orig = sprintf(str_orig, "% d", a);

  ck_assert_int_eq(res_8, res_8_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%5d", a);
  int res_9_orig = sprintf(str_orig, "%5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_10 = s21_sprintf(str, "%05d", a);
  int res_10_orig = sprintf(str_orig, "%05d", a);

  ck_assert_int_eq(res_10, res_10_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%-5d", a);
  int res_11_orig = sprintf(str_orig, "%-5d", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_13 = s21_sprintf(str, "% 5d", a);
  int res_13_orig = sprintf(str_orig, "% 5d", a);

  ck_assert_int_eq(res_13, res_13_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%.5d", a);
  int res_14_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%.5d", a);
  int res_15_orig = sprintf(str_orig, "%.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "% .5d", a);
  int res_16_orig = sprintf(str_orig, "% .5d", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_17 = s21_sprintf(str, "%+.5d", a);
  int res_17_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_17, res_17_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%-.5d", a);
  int res_18_orig = sprintf(str_orig, "%-.5d", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_19 = s21_sprintf(str, "%10.5d", a);
  int res_19_orig = sprintf(str_orig, "%10.5d", a);

  ck_assert_int_eq(res_19, res_19_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_21 = s21_sprintf(str, "%-10.5d", a);
  int res_21_orig = sprintf(str_orig, "%-10.5d", a);

  ck_assert_int_eq(res_21, res_21_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%3.5d", a);
  int res_22_orig = sprintf(str_orig, "%3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_23 = s21_sprintf(str, "% 3.5d", a);
  int res_23_orig = sprintf(str_orig, "% 3.5d", a);

  ck_assert_int_eq(res_23, res_23_orig);
  ck_assert_str_eq(str, str_orig);

  int res_24 = s21_sprintf(str, "%+3.5d", a);
  int res_24_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_24, res_24_orig);
  ck_assert_str_eq(str, str_orig);

  int res_25 = s21_sprintf(str, "%-3.5d", a);
  int res_25_orig = sprintf(str_orig, "%-3.5d", a);

  ck_assert_int_eq(res_25, res_25_orig);
  ck_assert_str_eq(str, str_orig);

  int res_26 = s21_sprintf(str, "%+05d", a);
  int res_26_orig = sprintf(str_orig, "%+05d", a);

  ck_assert_int_eq(res_26, res_26_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test_6) {
  char str[80];
  char str_orig[80];
  int a = 23;

  int res_1 = s21_sprintf(str, "%+10.5d", a);
  int res_1_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%+3.5d", a);
  int res_3_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%+d", a);
  int res_4_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%+0d", a);
  int res_5_orig = sprintf(str_orig, "%+0d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_6 = s21_sprintf(str, "%-+d", a);
  int res_6_orig = sprintf(str_orig, "%-+d", a);

  ck_assert_int_eq(res_6, res_6_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%+5d", a);
  int res_9_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_11 = s21_sprintf(str, "%-+5d", a);
  int res_11_orig = sprintf(str_orig, "%-+5d", a);

  ck_assert_int_eq(res_11, res_11_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%+.5d", a);
  int res_14_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%+.5d", a);
  int res_15_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_16 = s21_sprintf(str, "% .5d", a);
  int res_16_orig = sprintf(str_orig, "% .5d", a);

  ck_assert_int_eq(res_16, res_16_orig);
  ck_assert_str_eq(str, str_orig);

  int res_18 = s21_sprintf(str, "%-+.5d", a);
  int res_18_orig = sprintf(str_orig, "%-+.5d", a);

  ck_assert_int_eq(res_18, res_18_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_21 = s21_sprintf(str, "%-+10.5d", a);
  int res_21_orig = sprintf(str_orig, "%-+10.5d", a);

  ck_assert_int_eq(res_21, res_21_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%+3.5d", a);
  int res_22_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_23 = s21_sprintf(str, "% 3.5d", a);
  int res_23_orig = sprintf(str_orig, "% 3.5d", a);

  ck_assert_int_eq(res_23, res_23_orig);
  ck_assert_str_eq(str, str_orig);

  int res_25 = s21_sprintf(str, "%-+3.5d", a);
  int res_25_orig = sprintf(str_orig, "%-+3.5d", a);

  ck_assert_int_eq(res_25, res_25_orig);
  ck_assert_str_eq(str, str_orig);

  int res_26 = s21_sprintf(str, "%+05d", a);
  int res_26_orig = sprintf(str_orig, "%+05d", a);

  ck_assert_int_eq(res_26, res_26_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(s21_sprintf_d_test_0) {
  char str[80];
  char str_orig[80];
  int a = 0;

  int res_1 = s21_sprintf(str, "%+10.5d", a);
  int res_1_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_1, res_1_orig);
  ck_assert_str_eq(str, str_orig);

  int res_3 = s21_sprintf(str, "%+3.5d", a);
  int res_3_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_3, res_3_orig);
  ck_assert_str_eq(str, str_orig);

  int res_4 = s21_sprintf(str, "%+d", a);
  int res_4_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_4, res_4_orig);
  ck_assert_str_eq(str, str_orig);

  int res_5 = s21_sprintf(str, "%05d", a);
  int res_5_orig = sprintf(str_orig, "%05d", a);

  ck_assert_int_eq(res_5, res_5_orig);
  ck_assert_str_eq(str, str_orig);

  int res_7 = s21_sprintf(str, "%+d", a);
  int res_7_orig = sprintf(str_orig, "%+d", a);

  ck_assert_int_eq(res_7, res_7_orig);
  ck_assert_str_eq(str, str_orig);

  int res_9 = s21_sprintf(str, "%+5d", a);
  int res_9_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_9, res_9_orig);
  ck_assert_str_eq(str, str_orig);

  int res_12 = s21_sprintf(str, "%+5d", a);
  int res_12_orig = sprintf(str_orig, "%+5d", a);

  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);

  int res_14 = s21_sprintf(str, "%+.5d", a);
  int res_14_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_14, res_14_orig);
  ck_assert_str_eq(str, str_orig);

  int res_15 = s21_sprintf(str, "%+.5d", a);
  int res_15_orig = sprintf(str_orig, "%+.5d", a);

  ck_assert_int_eq(res_15, res_15_orig);
  ck_assert_str_eq(str, str_orig);

  int res_20 = s21_sprintf(str, "%+10.5d", a);
  int res_20_orig = sprintf(str_orig, "%+10.5d", a);

  ck_assert_int_eq(res_20, res_20_orig);
  ck_assert_str_eq(str, str_orig);

  int res_22 = s21_sprintf(str, "%+3.5d", a);
  int res_22_orig = sprintf(str_orig, "%+3.5d", a);

  ck_assert_int_eq(res_22, res_22_orig);
  ck_assert_str_eq(str, str_orig);

  int res_26 = s21_sprintf(str, "%+05d", a);
  int res_26_orig = sprintf(str_orig, "%+05d", a);

  ck_assert_int_eq(res_26, res_26_orig);
  ck_assert_str_eq(str, str_orig);
}

END_TEST

START_TEST(test_s21_sprintf_d) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%d", 20),
                   sprintf(buffer_orig, "%d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%d", -20),
                   sprintf(buffer_orig, "%d", -20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%+d", 20),
                   sprintf(buffer_orig, "%+d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5d", 20),
                   sprintf(buffer_orig, "%5d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05d", 20),
                   sprintf(buffer_orig, "%05d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%*d", 5, 20),
                   sprintf(buffer_orig, "%*d", 5, 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5d", -20),
                   sprintf(buffer_orig, "%-5d", -20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "% d", 20),
                   sprintf(buffer_orig, "% d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%+5d", 20),
                   sprintf(buffer_orig, "%+5d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%+05d", 20),
                   sprintf(buffer_orig, "%+05d", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5hd", 20),
                   sprintf(buffer_orig, "%-5hd", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%hd", -20),
                   sprintf(buffer_orig, "%hd", -20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%ld", 20000000000000),
                   sprintf(buffer_orig, "%ld", 20000000000000));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5ld", -20000000000000),
                   sprintf(buffer_orig, "%-5ld", -20000000000000));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%d", 0), sprintf(buffer_orig, "%d", 0));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_c) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};
  for (int i = 0; i < 127; i++) {
    ck_assert_int_eq(s21_sprintf(buffer, "%c", i),
                     sprintf(buffer_orig, "%c", i));
    ck_assert_str_eq(buffer, buffer_orig);
  }

  for (wchar_t i = 0; i < 127; i++) {
    ck_assert_int_eq(s21_sprintf(buffer, "%lc", i),
                     sprintf(buffer_orig, "%lc", i));
    ck_assert_str_eq(buffer, buffer_orig);
  }
}

END_TEST

START_TEST(test_s21_sprintf_i) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%i", 1), sprintf(buffer_orig, "%i", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5i", 1),
                   sprintf(buffer_orig, "%5i", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05i", 1),
                   sprintf(buffer_orig, "%05i", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5i", 1),
                   sprintf(buffer_orig, "%-5i", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%i", 015),
                   sprintf(buffer_orig, "%i", 015));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%i", 0x15),
                   sprintf(buffer_orig, "%i", 0x15));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%i", 0), sprintf(buffer_orig, "%i", 0));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_e) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%e", 123456.789),
                   sprintf(buffer_orig, "%e", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%E", 123456.789),
                   sprintf(buffer_orig, "%E", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%#E", 123456.789),
                   sprintf(buffer_orig, "%#E", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%e", -123456.789),
                   sprintf(buffer_orig, "%e", -123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%.0e", 123456.789),
                   sprintf(buffer_orig, "%.0e", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%.2e", 123456.789),
                   sprintf(buffer_orig, "%.2e", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5e", 123456.789),
                   sprintf(buffer_orig, "%5e", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05e", 123456.789),
                   sprintf(buffer_orig, "%05e", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5e", -123456.789),
                   sprintf(buffer_orig, "%-5e", -123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%+e", 123456.789),
                   sprintf(buffer_orig, "%+e", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%Le", 0.0L),
                   sprintf(buffer_orig, "%Le", 0.0L));
  ck_assert_int_eq(s21_sprintf(buffer, "%e", 0.),
                   sprintf(buffer_orig, "%e", 0.));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%E", 0.),
                   sprintf(buffer_orig, "%E", 0.));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%e", 0.54321),
                   sprintf(buffer_orig, "%e", 0.54321));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%Le", 0.543212345678L),
                   sprintf(buffer_orig, "%Le", 0.543212345678L));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%Le", 123456.543212345L),
                   sprintf(buffer_orig, "%Le", 123456.543212345L));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%LE", 0.543212345678L),
                   sprintf(buffer_orig, "%LE", 0.543212345678L));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%LE", 0.0L),
                   sprintf(buffer_orig, "%LE", 0.0L));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_f) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%f", 123456.789),
                   sprintf(buffer_orig, "%f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%f", -123456.789),
                   sprintf(buffer_orig, "%f", -123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%.2f", 123456.789),
                   sprintf(buffer_orig, "%.2f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%.2f", -123456.789),
                   sprintf(buffer_orig, "%.2f", -123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5f", 123456.789),
                   sprintf(buffer_orig, "%5f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5f", -123456.789),
                   sprintf(buffer_orig, "%5f", -123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05f", 123456.789),
                   sprintf(buffer_orig, "%05f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5f", 123456.789),
                   sprintf(buffer_orig, "%-5f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5.2f", 123456.789),
                   sprintf(buffer_orig, "%5.2f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%*.*f", 5, 2, 123456.789),
                   sprintf(buffer_orig, "%*.*f", 5, 2, 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%+f", 123456.789),
                   sprintf(buffer_orig, "%+f", 123456.789));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%f", 0.),
                   sprintf(buffer_orig, "%f", 0.));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%Lf", 1234567890.1234567890L),
                   sprintf(buffer_orig, "%Lf", 1234567890.1234567890L));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_g) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};
  int size = 1000;

  ck_assert_int_eq(s21_sprintf(buffer, "%g", PI),
                   sprintf(buffer_orig, "%g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%G", PI),
                   sprintf(buffer_orig, "%G", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%.2g", PI),
                   sprintf(buffer_orig, "%.2g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5g", PI),
                   sprintf(buffer_orig, "%5g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05g", PI),
                   sprintf(buffer_orig, "%05g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5g", PI),
                   sprintf(buffer_orig, "%-5g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%+-5g", PI),
                   sprintf(buffer_orig, "%+-5g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5.2g", PI),
                   sprintf(buffer_orig, "%5.2g", PI));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%Lg", 1234567890.1234567890L),
                   sprintf(buffer_orig, "%Lg", 1234567890.1234567890L));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%Lg", 0.543212345678L),
                   sprintf(buffer_orig, "%Lg", 0.543212345678L));
  ck_assert_str_eq(buffer, buffer_orig);

  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);
  ck_assert_int_eq(s21_sprintf(buffer, "%Lg", 0.0L),
                   sprintf(buffer_orig, "%Lg", 0.0L));
  ck_assert_str_eq(buffer, buffer_orig);

  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);
  ck_assert_int_eq(s21_sprintf(buffer, "%g", 0.),
                   sprintf(buffer_orig, "%g", 0.));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_o) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};
  const int size = 1000;

  ck_assert_int_eq(s21_sprintf(buffer, "%#o", 20),
                   sprintf(buffer_orig, "%#o", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%#ho", 20),
                   sprintf(buffer_orig, "%#ho", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%ho", 0),
                   sprintf(buffer_orig, "%ho", 0));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%#lo", 12345678900),
                   sprintf(buffer_orig, "%#lo", 12345678900));
  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);

  ck_assert_int_eq(s21_sprintf(buffer, "%lo", 0L),
                   sprintf(buffer_orig, "%lo", 0L));
  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);

  ck_assert_int_eq(s21_sprintf(buffer, "%05o", 20),
                   sprintf(buffer_orig, "%05o", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5o", 20),
                   sprintf(buffer_orig, "%-5o", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%o", 0), sprintf(buffer_orig, "%o", 0));
  ck_assert_str_eq(buffer, buffer_orig);

  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);

  ck_assert_int_eq(s21_sprintf(buffer, "%ho", (short int)20),
                   sprintf(buffer_orig, "%ho", (short int)20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%lo", 12312312320),
                   sprintf(buffer_orig, "%lo", 12312312320));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_s) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%s", "hihihi"),
                   sprintf(buffer_orig, "%s", "hihihi"));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5s", "ИГОРЬ, РАЗРЕШИ CTYPE"),
                   sprintf(buffer_orig, "%5s", "ИГОРЬ, РАЗРЕШИ CTYPE"));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5ls", L"TEST STRING"),
                   sprintf(buffer_orig, "%5ls", L"TEST STRING"));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_u) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%u", 1), sprintf(buffer_orig, "%u", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%5u", 1),
                   sprintf(buffer_orig, "%5u", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05u", 1),
                   sprintf(buffer_orig, "%05u", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%-5u", 1),
                   sprintf(buffer_orig, "%-5u", 1));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%hu", 20),
                   sprintf(buffer_orig, "%hu", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%lu", 20000000000000),
                   sprintf(buffer_orig, "%lu", 20000000000000));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_x) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};
  int size = 1000;

  ck_assert_int_eq(s21_sprintf(buffer, "%x", 20),
                   sprintf(buffer_orig, "%x", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%hx", 0),
                   sprintf(buffer_orig, "%hx", 0));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%x", 0), sprintf(buffer_orig, "%x", 0));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%X", 20),
                   sprintf(buffer_orig, "%X", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%#X", 20),
                   sprintf(buffer_orig, "%#X", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%#x", 20),
                   sprintf(buffer_orig, "%#x", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%05x", 20),
                   sprintf(buffer_orig, "%05x", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%lx", 0L),
                   sprintf(buffer_orig, "%lx", 0L));
  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);

  ck_assert_int_eq(s21_sprintf(buffer, "%-5x", 20),
                   sprintf(buffer_orig, "%-5x", 20));
  ck_assert_str_eq(buffer, buffer_orig);

  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);
  ck_assert_int_eq(s21_sprintf(buffer, "%hx", 20),
                   sprintf(buffer_orig, "%hx", 20));
  ck_assert_str_eq(buffer, buffer_orig);
  ck_assert_int_eq(s21_sprintf(buffer, "%lx", 12345678900),
                   sprintf(buffer_orig, "%lx", 12345678900));
  ck_assert_str_eq(buffer, buffer_orig);
  memset(buffer, 0, size);
  memset(buffer_orig, 0, size);
}

END_TEST

START_TEST(test_s21_sprintf_pro) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};

  ck_assert_int_eq(s21_sprintf(buffer, "%%"), sprintf(buffer_orig, "%%"));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_n) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};
  int count = 0;

  ck_assert_int_eq(
      s21_sprintf(buffer, "Тестовое сообщение. А тут%n мышь", &count),
      sprintf(buffer_orig, "Тестовое сообщение. А тут%n мышь", &count));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

START_TEST(test_s21_sprintf_p) {
  char buffer[1000] = {0};
  char buffer_orig[1000] = {0};
  int var = 10;
  int *ptr = &var;

  ck_assert_int_eq(s21_sprintf(buffer, "%p", (void *)ptr),
                   sprintf(buffer_orig, "%p", (void *)ptr));
  ck_assert_str_eq(buffer, buffer_orig);
}

END_TEST

END_TEST

int s21_sprintf_test() {
  Suite *suite = suite_create("s21_sprintf");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_sprintf_d);
  tcase_add_test(tc_core, test_s21_sprintf_c);
  tcase_add_test(tc_core, test_s21_sprintf_i);
  tcase_add_test(tc_core, test_s21_sprintf_e);
  tcase_add_test(tc_core, test_s21_sprintf_f);
  tcase_add_test(tc_core, test_s21_sprintf_g);
  tcase_add_test(tc_core, test_s21_sprintf_o);
  tcase_add_test(tc_core, test_s21_sprintf_s);
  tcase_add_test(tc_core, test_s21_sprintf_u);
  tcase_add_test(tc_core, test_s21_sprintf_x);
  tcase_add_test(tc_core, test_s21_sprintf_n);
  tcase_add_test(tc_core, test_s21_sprintf_p);
  tcase_add_test(tc_core, test_s21_sprintf_pro);
  tcase_add_test(tc_core, s21_sprintf_f_test);
  tcase_add_test(tc_core, s21_sprintf_n_test);
  tcase_add_test(tc_core, s21_sprintf_x_X_test);
  tcase_add_test(tc_core, s21_sprintf_c_test);
  tcase_add_test(tc_core, s21_sprintf_s_test);
  tcase_add_test(tc_core, s21_sprintf_u_test);
  tcase_add_test(tc_core, s21_sprintf_d_test);
  tcase_add_test(tc_core, s21_sprintf_d_test_2);
  tcase_add_test(tc_core, s21_sprintf_d_test_3);
  tcase_add_test(tc_core, s21_sprintf_d_test_4);
  tcase_add_test(tc_core, s21_sprintf_d_test_5);
  tcase_add_test(tc_core, s21_sprintf_d_test_6);
  tcase_add_test(tc_core, s21_sprintf_d_test_0);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_s21_strtok) {
  char str1[] = "This is a test string";
  const char delim1[] = " ";
  char str2[] = " ";
  const char delim2[] = "!";
  char str3[] = "This is a test string";
  const char delim3[] = "";

  ck_assert_str_eq(s21_strtok(str1, delim1), strtok(str1, delim1));
  ck_assert_str_eq(s21_strtok(str2, delim2), strtok(str2, delim2));
  ck_assert_ptr_eq(s21_strtok(str3, delim3), strtok(str3, delim3));
}

END_TEST

START_TEST(s21_strtok_test2) {
  char str_strtok1[] = "one/two/three(four)five";
  char delim1[] = "/()";

  char str_strtok2[] = "one/two/three(four)five";
  char delim2[] = "/()";
  char *my_strtok = s21_strtok(str_strtok1, delim1);
  char *origin_strtok = strtok(str_strtok2, delim2);
  while (my_strtok != S21_NULL) {
    ck_assert_str_eq(my_strtok, origin_strtok);
    my_strtok = s21_strtok(S21_NULL, delim1);
    origin_strtok = strtok(S21_NULL, delim2);
  }
}

END_TEST

START_TEST(strtok_1) {
  char str_24[15] = "This,is,my,way";
  char str_25[2] = ",";
  ck_assert_pstr_eq(strtok(str_24, str_25), s21_strtok(str_24, str_25));
}

END_TEST

START_TEST(strtok_3) {
  char str1[6] = "delim";
  char str2[2] = "f";
  ck_assert_pstr_eq(strtok(str1, str2), s21_strtok(str1, str2));
}

END_TEST

START_TEST(strtok_5) {
  char str[50] = "Per, :aspera;ad astra";
  char str1[10] = " ,:\n\0";
  ck_assert_pstr_eq(strtok(str, str1), s21_strtok(str, str1));
}

END_TEST

START_TEST(strtok_6) {
  char str[50] = "Per, :aspera;ad. astra";
  char str1[10] = ",:\n\0";
  ck_assert_pstr_eq(strtok(str, str1), s21_strtok(str, str1));
}

END_TEST

START_TEST(strtok_7) {
  char str[50] = "Per :aspn::::::era;ad. ast::::ra:";
  char str1[10] = ":";
  ck_assert_pstr_eq(strtok(str, str1), s21_strtok(str, str1));
}

END_TEST

START_TEST(strtok_8) {
  char str[50] = "test1/test2/test3/test4";
  char str1[10] = "/";
  ck_assert_pstr_eq(strtok(str, str1), s21_strtok(str, str1));
}

END_TEST

START_TEST(strtok_9) {
  char str[50] = "AadacAdxa";
  char str1[10] = "a";
  ck_assert_pstr_eq(strtok(str, str1), s21_strtok(str, str1));
}

END_TEST

START_TEST(strtok_11) {
  char str[] = "A";
  char str1[] = "\0";
  ck_assert_pstr_eq(strtok(str, str1), s21_strtok(str, str1));
}

END_TEST

START_TEST(strtok_13) {
  char str_24[15] = "This,is,my,way";
  char str_25[2] = ",";
  ck_assert_pstr_eq(strtok(str_24, str_25), s21_strtok(str_24, str_25));
}

END_TEST

START_TEST(strtok_15) {
  char str1[6] = "delim";
  char str2[2] = "f";
  ck_assert_pstr_eq(strtok(str1, str2), s21_strtok(str1, str2));
}

END_TEST

START_TEST(strtok_16) {
  char str1[] = "qwerty";
  char str2[] = "1234";
  ck_assert_pstr_eq(strtok(str1, str2), s21_strtok(str1, str2));
}

END_TEST

START_TEST(strtok_17) {
  char str1[22] = "Per, :aspera;ad astra";
  char str2[3] = " ";
  ck_assert_pstr_eq(strtok(str1, str2), s21_strtok(str1, str2));
}

END_TEST

START_TEST(strtok_18) {
  char str1[] = "check/Me/Out\\";
  char str2[] = "check/Me/Out\\";
  char simbols[] = "/";
  char *nstr_1, *nstr_2;
  nstr_1 = strtok(str1, simbols);
  nstr_2 = s21_strtok(str2, simbols);
  while (nstr_1 != S21_NULL) {
    ck_assert_pstr_eq(nstr_1, nstr_2);
    nstr_1 = strtok(S21_NULL, simbols);
    nstr_2 = s21_strtok(S21_NULL, simbols);
  }
}

END_TEST

START_TEST(strtok_20) {
  char str4_1[] = "testing with original string.h";
  char str4_2[] = "testing with original string.h";
  char del4[3] = ". ";
  char *nstr_1, *nstr_2;
  nstr_1 = strtok(str4_1, del4);
  nstr_2 = s21_strtok(str4_2, del4);
  while (nstr_1 != S21_NULL) {
    ck_assert_pstr_eq(nstr_1, nstr_2);
    nstr_1 = strtok(S21_NULL, del4);
    nstr_2 = s21_strtok(S21_NULL, del4);
  }
}

END_TEST

int s21_strtok_test() {
  Suite *suite = suite_create("s21_strtok");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_strtok);
  tcase_add_test(tc_core, strtok_1);
  tcase_add_test(tc_core, strtok_3);
  tcase_add_test(tc_core, strtok_5);
  tcase_add_test(tc_core, strtok_6);
  tcase_add_test(tc_core, strtok_7);
  tcase_add_test(tc_core, strtok_8);
  tcase_add_test(tc_core, strtok_9);
  tcase_add_test(tc_core, strtok_11);
  tcase_add_test(tc_core, strtok_13);
  tcase_add_test(tc_core, strtok_15);
  tcase_add_test(tc_core, strtok_16);
  tcase_add_test(tc_core, strtok_17);
  tcase_add_test(tc_core, strtok_18);
  tcase_add_test(tc_core, strtok_20);
  tcase_add_test(tc_core, s21_strtok_test2);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(test_strpbrk_basic) {
  const char *str1 = "Hello, World!";
  const char *str2 = "aeiou";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, 'e');
}

END_TEST

START_TEST(test_strpbrk_no_match) {
  const char *str1 = "Hello, World!";
  const char *str2 = "123";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_eq(result, NULL);
}

END_TEST

START_TEST(test_strpbrk_empty_str1) {
  const char *str1 = "";
  const char *str2 = "aeiou";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_eq(result, NULL);
}

END_TEST

START_TEST(test_strpbrk_empty_str2) {
  const char *str1 = "Hello, World!";
  const char *str2 = "";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_eq(result, NULL);
}

END_TEST

START_TEST(test_strpbrk_both_empty) {
  const char *str1 = "";
  const char *str2 = "";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_eq(result, NULL);
}

END_TEST

START_TEST(test_strpbrk_multiple_matches) {
  const char *str1 = "Hello, World!";
  const char *str2 = "lo";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, 'l');
}

END_TEST

START_TEST(test_strpbrk_match_at_start) {
  const char *str1 = "Hello, World!";
  const char *str2 = "HW";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, 'H');
}

END_TEST

START_TEST(test_strpbrk_match_at_end) {
  const char *str1 = "Hello, World!";
  const char *str2 = "!";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, '!');
}

END_TEST

START_TEST(test_strpbrk_match_empty_str2) {
  const char *str1 = "Hello, World!";
  const char *str2 = "";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_eq(result, NULL);
}

END_TEST

START_TEST(test_strpbrk_case_sensitive) {
  const char *str1 = "Hello, World!";
  const char *str2 = "HW";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, 'H');
}

END_TEST

START_TEST(test_strpbrk_case_insensitive) {
  const char *str1 = "Hello, World!";
  const char *str2 = "HW";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, 'H');
}

END_TEST

START_TEST(test_strpbrk_match_space) {
  const char *str1 = "Hello, World!";
  const char *str2 = " ";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, ' ');
}

END_TEST

START_TEST(test_strpbrk_match_symbols) {
  const char *str1 = "!@#$%^&*()";
  const char *str2 = "^";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, '^');
}

END_TEST

START_TEST(test_strpbrk_match_multiple_symbols) {
  const char *str1 = "!@#$%^&*()";
  const char *str2 = "$%";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, '$');
}

END_TEST

START_TEST(test_strpbrk_match_newline) {
  const char *str1 = "Hello, World!\n";
  const char *str2 = "\n";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, '\n');
}

END_TEST

START_TEST(test_strpbrk_match_tab) {
  const char *str1 = "Hello, World!\t";
  const char *str2 = "\t";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, '\t');
}

END_TEST

START_TEST(test_strpbrk_match_mixed) {
  const char *str1 = "Hello, World!";
  const char *str2 = "ld!";

  char *result = s21_strpbrk(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, 'l');
}

END_TEST

START_TEST(test_wcsstr_match_unicode) {
  const wchar_t *str1 = L"Hello, © World!";
  const wchar_t *str2 = L"©";

  wchar_t *result = wcsstr(str1, str2);

  ck_assert_ptr_ne(result, NULL);
  ck_assert_int_eq(*result, L'©');
}

END_TEST

int s21_strpbrk_test() {
  Suite *suite = suite_create("s21_strpbrk");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strpbrk_basic);
  tcase_add_test(tc_core, test_strpbrk_no_match);
  tcase_add_test(tc_core, test_strpbrk_empty_str1);
  tcase_add_test(tc_core, test_strpbrk_empty_str2);
  tcase_add_test(tc_core, test_strpbrk_both_empty);
  tcase_add_test(tc_core, test_strpbrk_multiple_matches);
  tcase_add_test(tc_core, test_strpbrk_match_at_start);
  tcase_add_test(tc_core, test_strpbrk_match_at_end);
  tcase_add_test(tc_core, test_strpbrk_match_empty_str2);
  tcase_add_test(tc_core, test_strpbrk_case_sensitive);
  tcase_add_test(tc_core, test_strpbrk_case_insensitive);
  tcase_add_test(tc_core, test_strpbrk_match_space);
  tcase_add_test(tc_core, test_strpbrk_match_symbols);
  tcase_add_test(tc_core, test_strpbrk_match_multiple_symbols);
  tcase_add_test(tc_core, test_strpbrk_match_newline);
  tcase_add_test(tc_core, test_strpbrk_match_tab);
  tcase_add_test(tc_core, test_strpbrk_match_mixed);
  tcase_add_test(tc_core, test_wcsstr_match_unicode);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

START_TEST(test_s21_memcmp) {
  // Сравнение одинаковых буферов
  {
    char buffer1[] = "Hello, World!";
    char buffer2[] = "Hello, World!";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer1));
    ck_assert_int_eq(result, 0);
  }

  // Сравнение разных буферов
  {
    char buffer1[] = "Hello, World!";
    char buffer2[] = "HELLO, WORLD!";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer1));
    ck_assert_int_gt(result, 0);  // buffer1 > buffer2
  }

  // Сравнение буферов разной длины
  {
    char buffer1[] = "Hello, World!";
    char buffer2[] = "Hello, World!123";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer2));
    ck_assert_int_eq(result, -49);
  }

  {
    char buffer1[] = "Hello, World!";
    char buffer2[] = "Hello, Worl";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer1));
    ck_assert_int_gt(result, 0);  // buffer1 > buffer2
  }

  // Сравнение с нулями внутри буфера
  {
    char buffer1[] = "Hello, World!";
    char buffer2[] = "Hello, World!\0\0\0";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer2));
    ck_assert_int_eq(result, 72);  // buffer1 > buffer2
  }

  {
    char buffer1[] = "Hello, World!\0\0\0";
    char buffer2[] = "Hello, World!";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer1));
    ck_assert_int_eq(result, -72);  // buffer1 < buffer2
  }

  // Сравнение нулевых буферов
  {
    char buffer1[] = "";
    char buffer2[] = "";

    int result = s21_memcmp(buffer1, buffer2, 0);
    ck_assert_int_eq(result, 0);
  }

  // Сравнение нулевого буфера и ненулевого
  {
    char buffer1[] = "";
    char buffer2[] = "Hello, World!";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer2));
    ck_assert_int_eq(result, -72);  // buffer1 > buffer2
  }

  {
    char buffer1[] = "Hello, World!";
    char buffer2[] = "";

    int result = s21_memcmp(buffer1, buffer2, sizeof(buffer1));
    ck_assert_int_gt(result, 0);  // buffer1 > buffer2
  }
}

END_TEST

int s21_memcmp_tests() {
  Suite *suite = suite_create("s21_memcmp");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_memcmp);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(insert_1) {
  char str1[7] = "this +";
  char str2[6] = " this";
  char *str3 = (char *)s21_insert(str1, str2, 6);
  ck_assert_str_eq(str3, "this + this");
  free(str3);
}

END_TEST

START_TEST(insert_2) {
  char str1[1] = "";
  char str2[14] = "checking null";
  char *str3 = (char *)s21_insert(str1, str2, 1);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(insert_3) {
  char str1[8] = "splitme";
  char str2[2] = " ";
  char *str3 = (char *)s21_insert(str1, str2, 5);
  ck_assert_str_eq(str3, "split me");
  free(str3);
}

END_TEST

START_TEST(insert_4) {
  char str1[14] = "checking null";
  char *str2 = S21_NULL;
  char *str3 = (char *)s21_insert(str1, str2, 0);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(insert_5) {
  char str1[4] = "end";
  char str2[6] = "start";
  char *str3 = (char *)s21_insert(str1, str2, 0);
  ck_assert_str_eq(str3, "startend");
  free(str3);
}

END_TEST

START_TEST(insert_6) {
  char str1[7] = "caseof";
  char str2[8] = "mistake";
  char *str3 = (char *)s21_insert(str1, str2, -1);
  ck_assert_ptr_eq(str3, NULL);
  free(str3);
}

END_TEST

START_TEST(insert_7) {
  char *str1 = S21_NULL;
  char str2[14] = "checking null";
  char *str3 = (char *)s21_insert(str1, str2, 1);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(insert_8) {
  char str1[2] = " ";
  char str2[32] = "if strlen src lesser than index";
  char *str3 = (char *)s21_insert(str1, str2, 3);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(insert_9) {
  char *str1 = S21_NULL;
  char str2[1] = "";
  char *str3 = (char *)s21_insert(str1, str2, 0);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(insert_10) {
  char *str1 = S21_NULL;
  char *str2 = S21_NULL;
  char *str3 = (char *)s21_insert(str1, str2, 0);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(insert_11) {
  char str1[4] = "";
  char str2[6] = "";
  char *str3 = (char *)s21_insert(str1, str2, 1);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(test_s21_insert) {
  const char *src1 = "Hello, World!";
  const char *str1 = "123";
  size_t start_index1 = 0;
  char *result1 = s21_insert(src1, str1, start_index1);
  ck_assert_str_eq(result1, "123Hello, World!");
  free(result1);

  const char *src2 = "Hello, World!";
  const char *str2 = "123";
  size_t start_index2 = 6;
  char *result2 = s21_insert(src2, str2, start_index2);
  ck_assert_str_eq(result2, "Hello,123 World!");
  free(result2);

  char *ptr = s21_insert("abc", "pp", 1);
  ck_assert_msg(memcmp(ptr, "appbc", 5) == 0, "FAILURE! Test \'abc\' failed!");
  free(ptr);

  ptr = s21_insert("abc", "pp", 0);
  ck_assert_msg(memcmp(ptr, "ppabc", 5) == 0, "FAILURE! Test \'abc\' failed!");
  free(ptr);

  ptr = s21_insert("abc", "pp", 3);
  ck_assert_msg(memcmp(ptr, "abcpp", 5) == 0, "FAILURE! Test \'abc\'  failed!");
  free(ptr);

  ptr = s21_insert("abc", "pp", 2);
  ck_assert_msg(memcmp(ptr, "abppc", 5) == 0,
                "FAILURE! Test \'abcdef\' failed!");
  free(ptr);

  ptr = s21_insert("15", "234", 1);
  ck_assert_msg(memcmp(ptr, "12345", 5) == 0, "FAILURE! Test \'15\' failed!");
  free(ptr);

  const char *src4 = "";
  const char *str4 = "123";
  size_t start_index4 = 0;
  char *result4 = s21_insert(src4, str4, start_index4);
  ck_assert_str_eq(result4, "123");
  free(result4);

  const char *src5 = "Hello, World!";
  const char *str5 = "";
  size_t start_index5 = 5;
  char *result5 = s21_insert(src5, str5, start_index5);
  ck_assert_str_eq(result5, "Hello, World!");
  free(result5);

  const char *src6 = NULL;
  const char *str6 = "123";
  size_t start_index6 = 0;
  char *result6 = s21_insert(src6, str6, start_index6);
  ck_assert_ptr_eq(result6, NULL);

  const char *src7 = "Hello, World!";
  const char *str7 = NULL;
  size_t start_index7 = 0;
  char *result7 = s21_insert(src7, str7, start_index7);
  ck_assert_ptr_eq(result7, NULL);

  const char *src11 = "Hello, World!";
  const char *str11 = "123";
  size_t start_index11 = 3;
  char *result11 = s21_insert(src11, str11, start_index11);
  ck_assert_str_eq(result11, "Hel123lo, World!");
  free(result11);

  const char *src12 = "Hello, World!";
  const char *str12 = "123";
  size_t start_index12 = 11;
  char *result12 = s21_insert(src12, str12, start_index12);
  ck_assert_str_eq(result12, "Hello, Worl123d!");
  free(result12);

  const char *src13 = "Hello, World!";
  const char *str13 = "123";
  size_t start_index13 = 5;
  char *result13 = s21_insert(src13, str13, start_index13);
  ck_assert_str_eq(result13, "Hello123, World!");
  free(result13);

  const char *src14 = "Hello, World!";
  const char *str14 = "123";
  size_t start_index14 = 12;
  char *result14 = s21_insert(src14, str14, start_index14);
  ck_assert_str_eq(result14, "Hello, World123!");
  free(result14);

  const char *src15 = "Hello, World!";
  const char *str15 = "123";
  size_t start_index15 = 1;
  char *result15 = s21_insert(src15, str15, start_index15);
  ck_assert_str_eq(result15, "H123ello, World!");
  free(result15);

  const char *src17 = "Hello, World!";
  const char *str17 = "123";
  size_t start_index17 = 0;
  char *result17 = s21_insert(src17, str17, start_index17);
  ck_assert_str_eq(result17, "123Hello, World!");
  free(result17);

  const char *src18 = "Hello, World!";
  const char *str18 = "123";
  size_t start_index18 = 6;
  char *result18 = s21_insert(src18, str18, start_index18);
  ck_assert_str_eq(result18, "Hello,123 World!");
  free(result18);

  const char *src20 = "";
  const char *str20 = "";
  size_t start_index20 = 0;
  char *result20 = s21_insert(src20, str20, start_index20);
  ck_assert_str_eq(result20, "");
  free(result20);
}

END_TEST

START_TEST(test_s21_insert_append) {
  const char *src = "Hello, ";
  const char *str = "world!";
  s21_size_t start_index = 7;

  char *result = s21_insert(src, str, start_index);

  ck_assert_str_eq(result, "Hello, world!");
  free(result);
}

END_TEST

START_TEST(test_s21_insert_null_check) {
  const char *src = "Hello, ";
  const char *str = NULL;
  s21_size_t start_index = 7;

  char *result = s21_insert(src, str, start_index);

  ck_assert_ptr_null(result);
  // Освобождаем, если не NULL
  if (result != NULL) {
    free(result);
  }
}

END_TEST

START_TEST(test_s21_insert_append_end) {
  const char *src = "Hello, ";
  const char *str = "world!";
  s21_size_t start_index = 100;  // Больше длины строки src

  char *result = s21_insert(src, str, start_index);
  ck_assert_ptr_eq(result, S21_NULL);
  free(result);
}

END_TEST

int s21_insert_test() {
  Suite *suite = suite_create("s21_insert");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_insert);
  tcase_add_test(tc_core, test_s21_insert_append);
  tcase_add_test(tc_core, test_s21_insert_null_check);
  tcase_add_test(tc_core, test_s21_insert_append_end);
  tcase_add_test(tc_core, insert_1);
  tcase_add_test(tc_core, insert_2);
  tcase_add_test(tc_core, insert_3);
  tcase_add_test(tc_core, insert_4);
  tcase_add_test(tc_core, insert_5);
  tcase_add_test(tc_core, insert_6);
  tcase_add_test(tc_core, insert_7);
  tcase_add_test(tc_core, insert_8);
  tcase_add_test(tc_core, insert_9);
  tcase_add_test(tc_core, insert_10);
  tcase_add_test(tc_core, insert_11);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

START_TEST(trim_1) {
  char *str1 = "Hello World";
  char *str2 = "H";
  char *str3 = (char *)s21_trim(str1, str2);
  ck_assert_str_eq(str3, "ello World");
}

END_TEST

START_TEST(trim_3) {
  char *str1 = "";
  char *str2 = "";
  char *str3 = (char *)s21_trim(str1, str2);
  ck_assert_str_eq(str3, "");
}

END_TEST

START_TEST(trim_4) {
  char *str = "empty";
  char *str2 = "empty";
  char *str3 = (char *)s21_trim(str, str2);
  ck_assert_str_eq(str3, "");
}

END_TEST

START_TEST(trim_5) {
  char *str = S21_NULL;
  char *str2 = S21_NULL;
  char *str3 = (char *)s21_trim(str, str2);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(trim_6) {
  char *str = "";
  char *str2 = S21_NULL;
  char *str3 = (char *)s21_trim(str, str2);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(trim_7) {
  char *str = S21_NULL;
  char *str2 = "          ";
  char *str3 = (char *)s21_trim(str, str2);
  ck_assert_ptr_eq(str3, S21_NULL);
  free(str3);
}

END_TEST

START_TEST(trim_8) {
  char *str = "empty";
  char *str2 = "t";
  char *str3 = (char *)s21_trim(str, str2);
  ck_assert_str_ne(str3, "em");
  free(str3);
}

END_TEST

START_TEST(S21_TRIM_FOUND_1) {
  generic_trim_tests("_xXx_Start_xXx_", "_xXx_", "Start");
}

END_TEST

START_TEST(S21_TRIM_FOUND_2) {
  generic_trim_tests("_xXx_Start_xXx_BN_xXx_", "_xXx_", "Start_xXx_BN");
}

END_TEST

START_TEST(S21_TRIM_NOT_FOUND_2) {
  generic_trim_tests("Start_xXx_sdasdasd", "_xXx_", "Start_xXx_sdasdasd");
}

END_TEST

void generic_trim_tests(const char *src, const char *trim_chars,
                        const char *check) {
  char *test = s21_trim(src, trim_chars);
  ck_assert_str_eq(test, check);
  //  free(test);
}

int s21_trim_tests() {
  Suite *suite = suite_create("s21_trim");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, S21_TRIM_FOUND_1);
  tcase_add_test(tc_core, S21_TRIM_FOUND_2);
  tcase_add_test(tc_core, S21_TRIM_NOT_FOUND_2);
  tcase_add_test(tc_core, trim_1);
  tcase_add_test(tc_core, trim_3);
  tcase_add_test(tc_core, trim_4);
  tcase_add_test(tc_core, trim_5);
  tcase_add_test(tc_core, trim_6);
  tcase_add_test(tc_core, trim_7);
  tcase_add_test(tc_core, trim_8);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return number_failed;
}

START_TEST(S21_MEMCHR_EMPTY) { generic_memchr_test("", 'b', 10); }

END_TEST

START_TEST(S21_MEMCHR_OK) { generic_memchr_test("sdasdasdas", 'a', 10); }

END_TEST

START_TEST(S21_MEMCHR_NOTHING_FOUND) {
  generic_memchr_test("hjekjwek", 'a', 10);
}

END_TEST

START_TEST(memchr_1) {
  char *s1 = "0";
  ck_assert_pstr_eq(memchr(s1, 48, 1), s21_memchr(s1, 48, 1));
}

END_TEST

START_TEST(memchr_2) {
  char str_9[12] = "Hello world";
  ck_assert_str_eq(memchr(str_9, 'w', strlen(str_9)),
                   s21_memchr(str_9, 'w', s21_strlen(str_9)));
}

END_TEST

START_TEST(memchr_3) {
  char str_3[12] = "i am\nproger";
  ck_assert_str_eq(memchr(str_3, '\n', 6), s21_memchr(str_3, '\n', 6));
}

END_TEST

START_TEST(memchr_4) {
  char str_5[5] = " \n\0\0";
  ck_assert_str_eq(memchr(str_5, '\0', 4), s21_memchr(str_5, '\0', 4));
}

END_TEST

START_TEST(memchr_6) {
  char str_1[12] = "Hello world";
  ck_assert_str_eq(memchr(str_1, 'w', strlen(str_1)),
                   s21_memchr(str_1, 'w', s21_strlen(str_1)));
}

END_TEST

START_TEST(memchr_8) {
  char str_1[20] = "Per aspera ad astra";
  ck_assert_ptr_eq(memchr(str_1, 'p', 2), s21_memchr(str_1, 'p', 2));
}

END_TEST

START_TEST(memchr_9) {
  char str_1[4] = "a\n\0";
  ck_assert_str_eq(memchr(str_1, '\0', 4), s21_memchr(str_1, '\0', 4));
}

END_TEST

START_TEST(memchr_10) {
  char *s1 = "0";
  ck_assert_pstr_eq(memchr(s1, 2, 4), s21_memchr(s1, 2, 4));
}

END_TEST

START_TEST(memchr_11) {
  char str_3[12] = "i am\nproger";
  ck_assert_ptr_eq(memchr(str_3, 'p', 2), s21_memchr(str_3, 'p', 2));
}

END_TEST

START_TEST(memchr_12) {
  char *s1 = "0";
  ck_assert_pstr_eq(memchr(s1, 85, 1), s21_memchr(s1, 85, 1));
}

END_TEST

START_TEST(memchr_13) {
  char str[] = "Hello w\0orld\0";
  ck_assert_pstr_eq(memchr(str, 'r', 5), s21_memchr(str, 'r', 5));
}

END_TEST

START_TEST(memchr_14) {
  char str2[] = "john.smith@microsoft.com";
  ck_assert_pstr_eq(memchr(str2, 's', 6), s21_memchr(str2, 's', 6));
}

END_TEST

START_TEST(memchr_15) {
  char str3[] = {'q', 'r', 's', 't', 'w', '\0'};
  ck_assert_pstr_eq(memchr(str3, 's', 5), s21_memchr(str3, 's', 5));
}

END_TEST

int s21_memchr_test() {
  Suite *suite = suite_create("s21_memchr");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, S21_MEMCHR_EMPTY);
  tcase_add_test(tc_core, S21_MEMCHR_OK);
  tcase_add_test(tc_core, S21_MEMCHR_NOTHING_FOUND);
  tcase_add_test(tc_core, memchr_1);
  tcase_add_test(tc_core, memchr_2);
  tcase_add_test(tc_core, memchr_3);
  tcase_add_test(tc_core, memchr_4);
  tcase_add_test(tc_core, memchr_6);
  tcase_add_test(tc_core, memchr_8);
  tcase_add_test(tc_core, memchr_9);
  tcase_add_test(tc_core, memchr_10);
  tcase_add_test(tc_core, memchr_11);
  tcase_add_test(tc_core, memchr_12);
  tcase_add_test(tc_core, memchr_13);
  tcase_add_test(tc_core, memchr_14);
  tcase_add_test(tc_core, memchr_15);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return number_failed;
}

void generic_memchr_test(const char *str, char chr, size_t n) {
  char *str_memchr = (char *)memchr(str, chr, n);
  char *s_memchr = (char *)s21_memchr(str, chr, n);
  ck_assert_ptr_eq(str_memchr, s_memchr);
}
