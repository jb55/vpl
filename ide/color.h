
#ifndef VPL_COLOR_H
#define VPL_COLOR_H


struct vpl_color {
  union {
    float rgba[4];
    struct {
      float r, g, b, a;
    };
  };
};


#endif /* VPL_COLOR_H */
