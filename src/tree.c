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

NODE *tree_put_operation(NODE *root, const char *operation, const char *value) {
  NODE *operation_node = tree_node_init(OPERATOR);
  tree_node_set_str(operation_node, operation);

  NODE *value_node = tree_node_init(INTEGER);
  tree_node_set_str(value_node, value);

  operation_node->left = root;
  operation_node->right = value_node;
  return operation_node;
}
