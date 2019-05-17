#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>

#include "consts.h"

enum {
  NODE_LEFT = 0,
  NODE_RIGHT,
};

typedef int side_t;

enum {
  OPERATOR = 0,
  INTEGER,
};

typedef int node_t;

typedef struct NODE {
  node_t nodetype;
  char value[LEXEME_MAX_SIZE];
  struct NODE *left;
  struct NODE *right;
} NODE;

void tree_node_init(NODE *node);
void tree_node_free(NODE *node);
void tree_add(NODE node, NODE *son, side_t side);

#endif
