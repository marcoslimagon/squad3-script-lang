#include "../src/tree.h"
#include <check.h>

START_TEST(test_init_node) {
  NODE node;
  ck_assert_int_eq(1024, sizeof(node.value));
}
END_TEST

START_TEST(test_read_node) {
  NODE node;
  NODE father;

  tree_add_left(father, &node);


  ck_assert_int_eq(father.left, &node);
}
END_TEST

Suite *tree_suite(void) {
  Suite *suite;
  TCase *tc_tree;

  tc_tree = tcase_create("init tree");
  tcase_add_test(tc_tree, test_init_node);
  tcase_add_test(tc_tree, test_read_node);

  suite = suite_create("Tree");



  return suite;
}

int main() {
  int number_failed;
  Suite *suite;
  SRunner *suite_runner;

  suite = tree_suite();
  suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return number_failed;
}
