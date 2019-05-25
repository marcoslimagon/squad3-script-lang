#ifndef __PARSER_H__
#define __PARSER_H__

/** SQD3 EBNF
 *
 * expr -> ['-'] factor { binary_op factor }
 * binary_op -> '+' | '-' | '*' | '/'
 * factor -> 'UINT' | '(' expr ')'
 *
 */

#include <stdlib.h>

#include "lexer.h"
#include "sqd3_types.h"
#include "tree.h"

integer expr(void);
integer factor(void);

#endif
