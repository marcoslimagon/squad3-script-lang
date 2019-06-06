#include "parser.h"

SQD3_OBJECT *expr(void) {
  char number[1024];

  SQD3_OBJECT *expr_result = factor();
  sprintf(number, "%lld", read_integer_from_object(expr_result));

  NODE *expr_root = tree_node_init(INTEGER);
  tree_node_set_str(expr_root, number);

  while (get_lookahead() == BINARY_OP) {
    char lexeme[2];
    read_lexeme(lexeme);
    match(BINARY_OP);

    SQD3_OBJECT *partial = factor();
    sprintf(number, "%lld", read_integer_from_object(partial));

    expr_root = tree_put_operation(expr_root, lexeme, number);
  }

  SQD3_OBJECT *tree_result = calculate_tree(expr_root);
  tree_node_free(expr_root);

  return tree_result;
}

SQD3_OBJECT *factor(void) {
  SQD3_OBJECT *result;

  if (get_lookahead() == START_PARENTHESES) {
    match(START_PARENTHESES);
    result = expr();
    match(END_PARENTHESES);
    return result;
  }

  char lexeme[LEXEME_MAX_SIZE];
  read_lexeme(lexeme);
  match(UINT);
  return integer_from_long_long(atoll(lexeme));
}
