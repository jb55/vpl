
#include "node.h"
#include "prelude.h"

struct vpl_node *
vpl_node_init(struct vpl_node *node)
{
  node->roundness = 10.f;
  node->padding = 8.f;
  node->pin_padding = 12.f;
  node->title = "node";
  node->x = 0;
  node->y = 0;
  node->w = 100;
  node->h = 100;
  node->border_color.r = 0;
  node->border_color.g = 0;
  node->border_color.b = 0;
  node->border_color.a = 1;
  node->left_pin_count = 0;
  node->right_pin_count = 0;
  node->bottom_pin_count = 0;
  node->left_pins = NULL;
  node->right_pins = NULL;
  node->bottom_pins = NULL;
  return node;
}


struct vpl_pin *
vpl_pin_init(struct vpl_pin *pin) {
  pin->border_color.r = 1;
  pin->border_color.g = 0;
  pin->border_color.b = 0;
  pin->border_color.a = 1;
  pin->color.r = 0;
  pin->color.g = 0;
  pin->color.b = 0;
  pin->color.a = 1;
  pin->graphic = vpl_pin_graphic_circle;
  pin->size = 12;
  pin->x = 0;
  pin->y = 0;
  return pin;
}
