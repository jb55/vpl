
#include "node.h"

struct vpl_node *
vpl_node_init(struct vpl_node *node)
{
  node->roundness = 10.f;
  node->padding = 2.f;
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


struct vpl_pin *
vpl_pin_init(struct vpl_pin *pin) {
  pin->border_color.r = 0;
  pin->border_color.g = 0;
  pin->border_color.b = 0;
  pin->border_color.a = 1;
  pin->color.r = 1;
  pin->color.g = 0;
  pin->color.b = 0;
  pin->color.a = 0;
  pin->side = vpl_side_left;
  pin->graphic = vpl_pin_graphic_circle;
  pin->size = 1;
  return pin;
}
