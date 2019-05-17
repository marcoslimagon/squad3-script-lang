#include "tree.h"

void tree_node_init(NODE *node, node_t nodetype) {
  node = malloc(sizeof(NODE));
  node->nodetype = nodetype;
  node->left = NULL;
  node->right = NULL;
}

void tree_node_free(NODE *node) {
  if (node->left != NULL)
    tree_node_free(node->left);

  if (node->right != NULL)
    tree_node_free(node->right);

  free(node);
}

void tree_add(NODE *node, NODE *son, side_t side) {
  if (side == NODE_LEFT)
    node->left = son;
  if (side == NODE_RIGHT)
    node->right = son;
}

void print_tree(NODE *node, char *str) {
  char value[1024];

  if (node->left != NULL)
    print_tree(node->left, str);

  if (node->right != NULL)
    print_tree(node->right, str);

  if (node->nodetype == OPERATOR)
    sprintf(value, "%s %s ", str, node->value);

  if (node->nodetype == INTEGER)
    sprintf(value, "%s%s", str, node->value);
}
