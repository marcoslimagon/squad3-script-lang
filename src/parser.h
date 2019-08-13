#ifndef __PARSER_H__
#define __PARSER_H__

/** SQD3 EBNF
 * stmt_list -> stmt { ';' stmt }
 * stmt -> expr
 * expr_list -> expr { ',' expr }
 * expr -> factor { binary_op factor }
 * binary_op -> '+' | '-' | '*' | '/' | '='
 * factor -> ['-']('UINT' | 'FLOAT' | 'ID' ['(' expr_list ')'] | '(' expr ')')
 */

#include <stdlib.h>

#include "lexer.h"
#include "sqd3_types.h"
#include "tree.h"

SQD3_OBJECT *stmt_list(void);
SQD3_OBJECT *stmt(void);
void expr_list(void);
SQD3_OBJECT *expr(void);
SQD3_OBJECT *factor(void);

#endif
