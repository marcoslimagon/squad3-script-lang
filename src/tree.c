#include "tree.h"

NODE *tree_node_init(node_t nodetype) {
  NODE *node = malloc(sizeof(NODE));
  node->nodetype = nodetype;
  node->left = NULL;
  node->right = NULL;
  node->parent = NULL;

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
  son->parent = node;
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

integer calculate_tree(NODE *node) {
  integer left;
  integer right;

  if (node->left != NULL)
    left = calculate_tree(node->left);
  if (node->right != NULL)
    right = calculate_tree(node->right);

  if (node->nodetype == OPERATOR) {
    if (!strcmp(node->value, "*"))
      return left * right;
    if (!strcmp(node->value, "+"))
      return left + right;
  }

  return atoll(node->value);
}

void tree_node_set_str(NODE *node, const char *str) {
  strcpy(node->value, str);
}

NODE *more_left_node(NODE *root) {
  if (root->left != NULL) {
    return more_left_node(root->left);
  }
  return root;
}

NODE *tree_put_operation(NODE *root, const char *operation, const char *value) {
  NODE *operation_node = tree_node_init(OPERATOR);
  tree_node_set_str(operation_node, operation);

  NODE *value_node = tree_node_init(INTEGER);
  tree_node_set_str(value_node, value);

  if (!strcmp(operation, "*")) {
    NODE *old_left = more_left_node(root);
    NODE *old_left_parent = old_left->parent;

    tree_add(operation_node, value_node, NODE_RIGHT);
    tree_add(operation_node, old_left, NODE_LEFT);

    if (old_left_parent != NULL) {
      operation_node->parent = old_left_parent;
      old_left_parent->left = operation_node;
      return root;
    }
  } else {
    tree_add(operation_node, root, NODE_LEFT);
    tree_add(operation_node, value_node, NODE_RIGHT);
  }

  return operation_node;
}
