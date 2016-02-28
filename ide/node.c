
#include "node.h"

struct vpl_node *
vpl_node_init(struct vpl_node *node)
{
  node->roundness = 10.;
  node->title = "node";
  node->x = 0;
  node->y = 0;
  node->w = 100;
  node->h = 100;
  node->border_color.r = 0;
  node->border_color.g = 0;
  node->border_color.b = 0;
  node->border_color.a = 1;
  return node;
}
