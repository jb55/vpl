
#include "ide.h"
#include "node.h"
#include <string.h>

int blowup = 0;
int screenshot = 0;
int premult = 0;

static void vpl_ide_mouseup(struct vpl_ide *ide);
static void vpl_ide_mousedown(struct vpl_ide *ide, float mx, float my);

static struct vpl_node *
vpl_ide_hit_nodes(struct vpl_ide *ide, float mx, float my);

static int
vpl_ide_hit_node(struct vpl_node *node, float mx, float my);

static struct vpl_pin *
vpl_ide_hit_pin(struct vpl_node *node, float nx, float ny);


/**
 *  IDE initialization
 */
void
vpl_ide_init(struct vpl_ide *ide) {
  ide->interact_state = VPL_NSTATE_IDLE;
  ide->active_hit_x = 0;
  ide->active_hit_y = 0;
  ide->active_node  = 0;
  ide->num_nodes    = 0;
}


/**
 *  IDE Interaction
 *
 *  Handles mouse events in the IDE
 */
void
vpl_ide_interact(struct vpl_ide *ide, int m1, float mx, float my) {
  int m1_was_down = ide->interact_state & VPL_NSTATE_M1DOWN;

  if (!m1_was_down && m1 == 1) {
    vpl_ide_mousedown(ide, mx, my);
  }

  if (m1_was_down && m1 == 0) {
    vpl_ide_mouseup(ide);
  }

  if (ide->interact_state & VPL_NSTATE_M1DOWN)
  if (ide->active_node != 0) {
    ide->active_node->x = mx - ide->active_hit_x;
    ide->active_node->y = my - ide->active_hit_y;
  }
}


/**
 *  Called after a mouseup event
 */
static inline void
vpl_ide_mouseup(struct vpl_ide *ide) {
  ide->interact_state &= ~VPL_NSTATE_M1DOWN;
}


/**
 *  Called after a mousedown event
 */
static inline void
vpl_ide_mousedown(struct vpl_ide *ide, float mx, float my) {
  struct vpl_pin *hit_pin;
  ide->interact_state |= VPL_NSTATE_M1DOWN;
  ide->active_node = vpl_ide_hit_nodes(ide, mx, my);
  if (ide->active_node) {
    ide->active_hit_x = mx - ide->active_node->x;
    ide->active_hit_y = my - ide->active_node->y;

    // TODO: pin hit detection
    hit_pin = vpl_ide_hit_pin(ide->active_node,
                              ide->active_hit_x,
                              ide->active_hit_y);
  }
}


/**
 *  IDE hit nodes
 *
 *  Check to see if a point hits any of the IDE nodes. return the first node
 *  that is detected
 */
static struct vpl_node *
vpl_ide_hit_nodes(struct vpl_ide *ide, float mx, float my) {
  int i;
  struct vpl_node *node;

  for (i = 0; i < ide->num_nodes; i++) {
    node = &ide->nodes[i];
    if (node && vpl_ide_hit_node(node, mx, my)) {
      return node;
    }
  }

  return 0;
}


/**
 *  IDE hit node
 *
 *  Check to see if a point is within a specific node
 */
static inline int
vpl_ide_hit_node(struct vpl_node *node, float mx, float my) {
  // TODO: custom hit detection callback?
  return mx >= node->x &&
         mx <= node->x + node->w &&
         my >= node->y &&
         my <= node->y + node->h;
}



static inline struct vpl_pin *
vpl_ide_hit_pin(struct vpl_node *node, float nx, float ny) {
}
