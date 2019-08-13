#include <stdio.h>

#include "builtin_functions.h"
#include "debug.h"
#include "parser.h"

void execute_file(const char **argv) {

  FILE *buffer = fopen(argv[1], "r");
  init_lexer(buffer);
  stmt_list();
  fclose(buffer);
}

void repl() {
  for (;;) {

    printf("\n> ");
    char input[120];
    fgets(input, 120, stdin);

    FILE *buffer = fmemopen(input, strlen(input), "r");

    init_lexer(buffer);
    SQD3_OBJECT *result = stmt();
    char str_result[100];
    to_string(result, str_result);
    printf("%s \n", str_result);
  }
}

int main(int argc, const char **argv) {

  init_vtable();

  SQD3_OBJECT *print_ref = build_builtin_function_ref("print", &print_function);
  declare_local_variable("print", print_ref);

  argc > 1 ? execute_file(argv) : repl();

  return 0;
}
