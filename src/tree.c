#include "tree.h"

NODE *tree_node_init(node_t nodetype) {
  NODE *node = malloc(sizeof(NODE));
  node->nodetype = nodetype;
  node->left = NULL;
  node->right = NULL;

  return node;
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

  if (node->left != NULL) {
    print_tree(node->left, str);
  }

  if (node->nodetype == OPERATOR) {
    sprintf(str, "%s %s ", str, node->value);
  }

  if (node->right != NULL) {
    print_tree(node->right, str);
  }

  if (node->nodetype == INTEGER) {
    sprintf(str, "%s%s", str, node->value);
  }
}

void tree_node_set_str(NODE *node, const char *str) {
  strcpy(node->value, str);
}
