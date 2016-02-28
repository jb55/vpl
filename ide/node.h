#ifndef VPL_NODE_H
#define VPL_NODE_H

enum vpl_pin_graphic {
  vpl_pin_circle,
  vpl_pin_square,
  vpl_pin_star,
  vpl_pin_custom,
  VPL_PIN_GRAPHIC_COUNT
};

enum vpl_side {
  vpl_side_right,
  vpl_side_bottom,
  vpl_side_left,
  vpl_side_top
};

enum vpl_stroke {
  vpl_stroke_solid,
  vpl_stroke_dashed,
  VPL_STROKE_COUNT
};

struct vpl_color {
  union {
    float rgba[4];
    struct {
      float r, g, b, a;
    };
  };
};

struct vpl_pin {
  float size;
  enum vpl_pin_graphic graphic;
  struct vpl_color border_color;
  struct vpl_color color;
};

struct vpl_node {
  float roundness;
  char* title;
  float x, y, w, h;
  int num_pins;
  struct vpl_color border_color;
  struct vpl_pin *pins;
};

struct vpl_node *
vpl_node_init(struct vpl_node *node);

#endif /* VPL_NODE_H */
