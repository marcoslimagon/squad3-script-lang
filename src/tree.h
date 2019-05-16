#ifndef __TREE_H__
#define __TREE_H__

#include "consts.h"

typedef struct NODE {
    char value[LEXEME_MAX_SIZE];
    struct NODE* left;
    struct NODE* right;
} NODE;

void tree_add_left(NODE node, NODE *left);

#endif