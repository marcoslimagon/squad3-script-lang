#include "../src/parser.h"
#include "../src/sqd3_types.h"

#include <check.h>

/**
 * Fixture function
 */
SQD3_OBJECT *build1() { return integer_from_long_long(1); }

SQD3_OBJECT *soma() {
  SQD3_OBJECT *left = recover_from_stack_args(1);
  SQD3_OBJECT *right = recover_from_stack_args(0);
  return execute_operator_plus(left, right);
}

START_TEST(test_factor_integer) {
  char input[] = "1234";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = factor();
  ck_assert_int_eq(1234, read_integer_from_object(result));
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_only_factor) {
  char input[] = "1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 1);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_sum) {
  char input[] = "1 + 1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 2);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_minus) {
  char input[] = "1 - 1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 0);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_minus_negative) {
  char input[] = "1 - 2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), -1);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_minus_more_digits) {
  char input[] = "25 - 15";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 10);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_mult) {
  char input[] = "2 * 2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 4);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_div) {
  char input[] = "4 / 2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 2);
  fclose(buffer);
}
END_TEST

START_TEST(test_expr_mult_and_minus) {
  char input[] = "100*2 - 15";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 185);
  fclose(buffer);
}
END_TEST

START_TEST(test_complex_with_parentheses) {
  char input[] = "100*(4 + 4)";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 800);
  fclose(buffer);
}
END_TEST

START_TEST(test_complex_without_parentheses) {
  char input[] = "100*4 + 4";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  SQD3_OBJECT *result = expr();
  ck_assert_int_eq(read_integer_from_object(result), 404);
  fclose(buffer);
}
END_TEST

START_TEST(test_negative_expression) {
  char input[] = "- 10";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  ck_assert_int_eq(read_integer_from_object(expr()), -10);
  fclose(buffer);
}
END_TEST

START_TEST(test_negative_factor) {
  char input[] = "10 + - 9";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  ck_assert_int_eq(read_integer_from_object(expr()), 1);
  fclose(buffer);
}
END_TEST

START_TEST(test_negative_with_parentheses) {
  char input[] = "- (- 10 + (- 9))";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  ck_assert_int_eq(read_integer_from_object(expr()), 19);
  fclose(buffer);
}
END_TEST

START_TEST(test_factor_id) {
  char input[] = "10 + x";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_vtable();
  init_lexer(buffer);

  SQD3_OBJECT *x = integer_from_long_long(5);
  declare_local_variable("x", x);

  ck_assert_int_eq(read_integer_from_object(expr()), 15);
  fclose(buffer);
}
END_TEST

START_TEST(test_assign_variable) {
  char input[] = "x = 10";

  FILE *buffer = fmemopen(input, strlen(input), "r");

  init_vtable();

  init_lexer(buffer);
  ck_assert_int_eq(read_integer_from_object(expr()), 10);

  fclose(buffer);
}
END_TEST

START_TEST(test_assign_expression_result) {
  char input[] = "x = (10 + 20 * 5)";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);
  init_vtable();

  ck_assert_int_eq(read_integer_from_object(expr()), 110);
  VTABLE_ENTRY *entry = recover_variable("x");
  ck_assert_int_eq(read_integer_from_object(entry->ref), 110);
  fclose(buffer);
}
END_TEST

START_TEST(test_float_factor) {
  char input[] = "2.2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  ck_assert_float_eq(read_float_from_object(factor()), 2.2);
  fclose(buffer);
}
END_TEST

START_TEST(test_builtin_function) {
  char input[] = "build1() + 1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);
  init_vtable();

  SQD3_OBJECT *ref = build_builtin_function_ref("build1", &build1);
  declare_local_variable("build1", ref);

  ck_assert_int_eq(read_integer_from_object(expr()), 2);

  fclose(buffer);
}
END_TEST

START_TEST(test_builtin_function_with_parameters) {
  char input[] = "soma(1, 2)";
  char input2[] = "soma(3, 2)";

  FILE *buffer = fmemopen(input, strlen(input), "r");
  FILE *buffer2 = fmemopen(input2, strlen(input2), "r");

  init_vtable();
  SQD3_OBJECT *ref = build_builtin_function_ref("soma", &soma);
  declare_local_variable("soma", ref);

  init_lexer(buffer);
  ck_assert_int_eq(read_integer_from_object(expr()), 3);

  init_lexer(buffer2);
  ck_assert_int_eq(read_integer_from_object(expr()), 5);

  fclose(buffer);
  fclose(buffer2);
}
END_TEST

START_TEST(test_float_sum) {
  char input[] = "2.2 + 1.1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "3.300000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_and_int_sum) {
  char input[] = "1 + 2.2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "3.200000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_minus) {
  char input[] = "2.2 - 1.1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "1.100000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_and_int_minus) {
  char input[] = "2.2 - 1";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "1.200000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_multi) {
  char input[] = "4.4 * 2.0";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "8.800000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_and_int_multi) {
  char input[] = "2.2 * 2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "4.400000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_division) {
  char input[] = "4.4 / 2.0";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "2.200000");
  fclose(buffer);
}
END_TEST

START_TEST(test_float_and_int_division) {
  char input[] = "5.0 / 2";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  char destination[10];
  to_string(expr(), destination);
  ck_assert_str_eq(destination, "2.500000");
  fclose(buffer);
}
END_TEST

START_TEST(test_stmt) {
  char input[] = "- 10";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  ck_assert_int_eq(read_integer_from_object(stmt()), -10);
  fclose(buffer);
}
END_TEST

START_TEST(test_stmt_list_return) {
  char input[] = "- 10; 5";
  FILE *buffer = fmemopen(input, strlen(input), "r");
  init_lexer(buffer);

  ck_assert_int_eq(read_integer_from_object(stmt_list()), 5);
  fclose(buffer);
}
END_TEST

Suite *parser_suite(void) {
  Suite *suite;
  TCase *tc_factor;
  TCase *tc_expr;
  TCase *tc_complex_expr;
  TCase *tc_assgn_expr;
  TCase *tc_function_call;
  TCase *tc_stmt;

  suite = suite_create("Parser");
  tc_factor = tcase_create("factor");
  tc_expr = tcase_create("expr");
  tc_complex_expr = tcase_create("complex expr");
  tc_assgn_expr = tcase_create("assgn expr");
  tc_function_call = tcase_create("function call");
  tc_stmt = tcase_create("statement");

  tcase_add_test(tc_factor, test_factor_integer);
  tcase_add_test(tc_factor, test_factor_id);
  tcase_add_test(tc_factor, test_float_factor);

  tcase_add_test(tc_expr, test_expr_only_factor);
  tcase_add_test(tc_expr, test_expr_sum);
  tcase_add_test(tc_expr, test_expr_minus);
  tcase_add_test(tc_expr, test_expr_minus_negative);
  tcase_add_test(tc_expr, test_expr_minus_more_digits);
  tcase_add_test(tc_expr, test_expr_mult);
  tcase_add_test(tc_expr, test_expr_div);
  tcase_add_test(tc_expr, test_expr_mult_and_minus);
  tcase_add_test(tc_expr, test_negative_expression);
  tcase_add_test(tc_expr, test_negative_factor);
  tcase_add_test(tc_expr, test_negative_with_parentheses);
  tcase_add_test(tc_expr, test_float_sum);
  tcase_add_test(tc_expr, test_float_and_int_sum);
  tcase_add_test(tc_expr, test_float_minus);
  tcase_add_test(tc_expr, test_float_and_int_minus);
  tcase_add_test(tc_expr, test_float_multi);
  tcase_add_test(tc_expr, test_float_and_int_multi);
  tcase_add_test(tc_expr, test_float_division);
  tcase_add_test(tc_expr, test_float_and_int_division);

  tcase_add_test(tc_complex_expr, test_complex_with_parentheses);
  tcase_add_test(tc_complex_expr, test_complex_without_parentheses);

  tcase_add_test(tc_assgn_expr, test_assign_expression_result);
  tcase_add_test(tc_assgn_expr, test_assign_variable);

  tcase_add_test(tc_function_call, test_builtin_function);
  tcase_add_test(tc_function_call, test_builtin_function_with_parameters);

  tcase_add_test(tc_stmt, test_stmt);
  tcase_add_test(tc_stmt, test_stmt_list_return);

  suite_add_tcase(suite, tc_factor);
  suite_add_tcase(suite, tc_expr);
  suite_add_tcase(suite, tc_complex_expr);
  suite_add_tcase(suite, tc_assgn_expr);
  suite_add_tcase(suite, tc_function_call);
  suite_add_tcase(suite, tc_stmt);

  return suite;
}

int main() {
  int number_failed;
  Suite *suite;
  SRunner *suite_runner;

  suite = parser_suite();
  suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return number_failed;
}
