#include <stddef.h>
#include "../../string-helpers.h"
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>
#include "test.h"

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void) {
  return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void) {
  return 0;
}

/* The test initialization function.
 * Opens the temporary file used by the test.
 */
void init_test1(void) {
  return;
}

/* The test cleanup function.
 * Closes the temporary file used by the test in particular.
 */
void clean_test1(void) {
  return;
}

/* Simple test of is_digit().
 */
void testISDIGIT_Integers(void) {
  CU_ASSERT_FALSE(is_digit(0));
  CU_ASSERT_FALSE(is_digit(9));
  CU_ASSERT_FALSE(is_digit(-1));
}

void testISDIGIT_Digits(void) {
  CU_ASSERT_TRUE(is_digit('1'));
  CU_ASSERT_TRUE(is_digit('0'));
  CU_ASSERT_TRUE(is_digit('9'));
}

void testISDIGIT_EscChars(void) {
  CU_ASSERT_FALSE(is_digit('\0'));
  CU_ASSERT_FALSE(is_digit('\"'));
  CU_ASSERT_FALSE(is_digit('\n'));
}
/* Simple test of is_alpha().
 */
void testAlpha_letters(void) {
    CU_ASSERT_FALSE(is_alpha('_'));
}
void testAlpha_others(void) {
    CU_ASSERT_FALSE(is_alpha('1'));
    CU_ASSERT_FALSE(is_alpha('2'));
}
void testAlpha_proper(void) {
    CU_ASSERT_TRUE(is_alpha('A'));
    CU_ASSERT_TRUE(is_alpha('z'));
    CU_ASSERT_TRUE(is_alpha('c'));
    CU_ASSERT_TRUE(is_alpha('F'));
}
/* Simple test of is_space().
 */
void testSpace_letters(void) {
    CU_ASSERT_FALSE(is_space('a'));
    CU_ASSERT_FALSE(is_space('d'));
    CU_ASSERT_FALSE(is_space('e'));
}
void testSpace_others(void) {
    CU_ASSERT_FALSE(is_space('1'));
    CU_ASSERT_FALSE(is_space('2'));
    CU_ASSERT_FALSE(is_space('_'));
}
void testSpace_proper(void) {
    CU_ASSERT_TRUE(is_space(' '));
}
/* Simple test of is_identifier_component
 */
void testComponent_letters(void) {
    CU_ASSERT_TRUE(is_identifier_component('a'));
    CU_ASSERT_TRUE(is_identifier_component('d'));
    CU_ASSERT_TRUE(is_identifier_component('e'));
}
void testComponent_others(void) {
    CU_ASSERT_TRUE(is_identifier_component('1'));
    CU_ASSERT_TRUE(is_identifier_component('2'));
    CU_ASSERT_TRUE(is_identifier_component('_'));
}
void testComponent_false(void) {
    CU_ASSERT_FALSE(is_identifier_component(' '));
}

/* Simple test of is_valid_identifier
 */
void testIdentifier_letters(void) {
    char var = 'y';
    CU_ASSERT_TRUE(is_valid_identifier(&var));
}

void testIdentifier_underscore(void) {
    char var = '_';
    CU_ASSERT_FALSE(is_valid_identifier(&var));
}

void testIdentifier_others(void) {
    char var = ':';
    CU_ASSERT_FALSE(is_valid_identifier(&var));
}
/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
  CU_TestInfo isdigit_tests[] = {{"Test actual digits", testISDIGIT_Digits},
                                 {"Test esc chars", testISDIGIT_EscChars},
                                 {"Test numbers", testISDIGIT_Integers},
                                 CU_TEST_INFO_NULL};
  CU_TestInfo isalpha_tests[] = {{"Test actual letters", testAlpha_letters},
                                 {"Test others", testAlpha_others},
                                 {"Test properly", testAlpha_proper},
                                 CU_TEST_INFO_NULL};
  CU_TestInfo isspace_tests[] = {{"Test actual letters", testSpace_letters},
                                 {"Test others", testSpace_others},
                                 {"Test properly", testSpace_proper},
                                 CU_TEST_INFO_NULL};
  CU_TestInfo iscomponent_tests[] = {{"Test false", testComponent_false},
                                     {"Test others", testComponent_others},
                                     {"Test letters", testComponent_letters},
                                     CU_TEST_INFO_NULL};
  CU_TestInfo isidentifier_tests[] = {{"Test actual letters", testIdentifier_letters},
                                      {"Test others", testIdentifier_others},
                                      {"Test underscore", testIdentifier_underscore},
                                      CU_TEST_INFO_NULL};
  CU_SuiteInfo suites[] = {{"is_digit testing", init_suite1, clean_suite1,
                           isdigit_tests},
                           {"is_alpha testing", init_suite1, clean_suite1,
                               isalpha_tests},
                           {"is_space testing", init_suite1, clean_suite1,
                               isspace_tests},
                           {"is_component testing", init_suite1, clean_suite1,
                               iscomponent_tests},
                           {"is_identifier testing", init_suite1, clean_suite1,
                               isidentifier_tests},
                           CU_SUITE_INFO_NULL};

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  if (CU_register_suites(suites)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
