#ifndef __LEXER_H__
#define __LEXER_H__

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "tokens.h"
#include "consts.h"

// Essa funcao inicializa o lexer
void init_lexer(FILE *input);

// Essas funcoes serao utilizadas no parser
int get_lookahead(void);
void match(int expected);
void read_lexeme(char *dest);

#endif
