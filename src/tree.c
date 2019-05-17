#include "tree.h"

void tree_node_init(NODE *node) { node = malloc(sizeof(NODE)); }

void tree_add(NODE node, NODE *son, side_t side) {
  if (side == NODE_LEFT)
    node.left = son;
  if (side == NODE_RIGHT)
    node.right = son;
}
