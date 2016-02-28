#ifndef VPL_NODE_H
#define VPL_NODE_H


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
  struct vpl_color border_color;
};

struct vpl_node {
  float roundness;
  char* title;
  float x, y, w, h;
  struct vpl_color border_color;
};

struct vpl_node *
vpl_node_init(struct vpl_node *node);

#endif /* VPL_NODE_H */
