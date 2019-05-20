#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

NODE *tree_node_init(node_t nodetype);
void tree_node_free(NODE *node);
void tree_add(NODE *node, NODE *son, side_t side);
void print_tree(NODE *node, char *str);
void tree_node_set_str(NODE *node, const char *str);

#endif
