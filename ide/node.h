#ifndef VPL_NODE_H
#define VPL_NODE_H

#include "color.h"

enum vpl_pin_graphic {
  vpl_pin_graphic_circle,
  vpl_pin_graphic_square,
  vpl_pin_graphic_star,
  vpl_pin_graphic_custom
};

enum vpl_side {
  vpl_side_right,
  vpl_side_bottom,
  vpl_side_left,
  vpl_side_top
};

enum vpl_stroke {
  vpl_stroke_solid,
  vpl_stroke_dashed
};

struct vpl_pin {
  float size, x, y;
  enum vpl_pin_graphic graphic;
  struct vpl_node *parent;
  struct vpl_color border_color;
  struct vpl_color color;
};

struct vpl_node {
  float roundness;
  float padding;
  float pin_padding;
  char* title;
  float x, y, w, h;
  struct vpl_color border_color;

  // pins
  struct vpl_pin *left_pins;
  struct vpl_pin *right_pins;
  struct vpl_pin *bottom_pins;
  int left_pin_count;
  int right_pin_count;
  int bottom_pin_count;
};

struct vpl_node *
vpl_node_init(struct vpl_node *node);

struct vpl_pin *
vpl_pin_init(struct vpl_pin *pin);

#endif /* VPL_NODE_H */
