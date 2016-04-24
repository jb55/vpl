
#ifndef VPL_IDE_H
#define VPL_IDE_H

#include <nanovg/nanovg.h>

#define VPL_NSTATE_IDLE (1 << 0)
#define VPL_NSTATE_DRAG (1 << 1)

struct vpl_ide {
  NVGcontext *nvg;
  int m1, m2;
  int interact_state;
  struct vpl_node *active_node;
};

#endif /* VPL_IDE_H */
