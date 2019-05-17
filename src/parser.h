/** SQD3 EBNF
 *
 * expr -> ['-'] factor { binary_op factor }
 * binary_op -> '+' | '-' | '*' | '/'
 * factor -> 'UINT' | '(' expr ')'
 *
 */

#include <stdlib.h>

#include "lexer.h"
#include "tree.h"

typedef long long integer;

integer expr(void);
integer factor(void);
