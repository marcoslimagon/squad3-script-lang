#include "parser.h"

/**
 * private functions
 */
bool is_binary_op(token value) {
  return (value == '+' || value == '-' || value == '*' || value == '/' ||
          value == '=');
}

token unary_op(void) {
  token lookahead = get_lookahead();
  if (lookahead == '-' || lookahead == '!') {
    match(lookahead);
    return lookahead;
  }
  return 0;
}

SQD3_OBJECT *execute_function(varname_t function_name) {
  SQD3_OBJECT *(*function)();
  VTABLE_ENTRY *entry = recover_variable(function_name);

  if (entry == NULL) {
    fprintf(stderr, "error: function %s not declared\n", function_name);
    exit(-2);
  }

  void *function_ref = read_function_from_object(entry->ref);
  function = function_ref;

  return function();
}

token is_boolean() {
  if (get_lookahead() == TRUE || get_lookahead() == FALSE) {
    return get_lookahead();
  }
  return 0;
}

SQD3_OBJECT *booelean_object(token boolean) {
  if (boolean == TRUE) {
    return boolean_from_char(1);
  }
  return boolean_from_char(0);
}

/**
 * public functions
 */
SQD3_OBJECT *stmt_list(void) {
  SQD3_OBJECT *result = stmt();
  while (get_lookahead() == STMT_SEP) {
    match(STMT_SEP);
    result = stmt();
  }
  return result;
}

SQD3_OBJECT *stmt(void) { return expr(); }

void expr_list(void) {
  for (;;) {
    SQD3_OBJECT *result = expr();
    declare_local_variable("", result);
    if (get_lookahead() != COMMA)
      return;
    match(COMMA);
  }
}

SQD3_OBJECT *expr(void) {
  SQD3_OBJECT *expr_result = factor();

  NODE *expr_root = tree_node_init(VALUE);
  tree_node_set_value(expr_root, expr_result);

  while (is_binary_op(get_lookahead())) {
    operator_t op;
    read_lexeme(op);

    match(get_lookahead());

    SQD3_OBJECT *partial = factor();
    expr_root = tree_put_operation(expr_root, op, partial);
  }

  SQD3_OBJECT *tree_result = calculate_tree(expr_root);
  tree_node_free(expr_root);

  return tree_result;
}

SQD3_OBJECT *factor(void) {
  SQD3_OBJECT *result;
  token invert_factor = unary_op();

  if (get_lookahead() == START_PARENTHESES) {
    match(START_PARENTHESES);
    result = expr();

    match(END_PARENTHESES);
    return execute_operator_unary_op(invert_factor, result);
  }

  char lexeme[LEXEME_MAX_SIZE];
  read_lexeme(lexeme);

  if (get_lookahead() == UINT) {
    match(UINT);
    return execute_operator_unary_op(invert_factor,
                                     integer_from_long_long(atoll(lexeme)));
  }

  if (get_lookahead() == FLOAT) {
    match(FLOAT);
    return execute_operator_unary_op(invert_factor,
                                     float_from_float(atof(lexeme)));
  }

  token boolean;
  if ((boolean = is_boolean())) {
    match(boolean);
    return execute_operator_unary_op(invert_factor, booelean_object(boolean));
  }

  match(ID);
  if (get_lookahead() == START_PARENTHESES) {
    init_context(lexeme);

    match(START_PARENTHESES);
    if (get_lookahead() != END_PARENTHESES) {
      expr_list();
    }
    match(END_PARENTHESES);

    SQD3_OBJECT *function_result = execute_function(lexeme);
    dispose_local_variables();
    finish_context();

    return execute_operator_unary_op(invert_factor, function_result);
  }
  return execute_operator_unary_op(invert_factor, build_ref(lexeme));
}
