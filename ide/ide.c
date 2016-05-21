
#include "ide.h"
#include "node.h"
#include "wire.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int blowup = 0;
int screenshot = 0;
int premult = 0;

static void vpl_ide_mouseup(struct vpl_ide *ide, float mx, float my);
static void vpl_ide_mousedown(struct vpl_ide *ide, float mx, float my);

static struct vpl_node *
vpl_ide_hit_nodes(struct vpl_ide *ide, float mx, float my);

static int
vpl_ide_hit_node(struct vpl_node *node, float mx, float my);

static int
vpl_ide_hit_box(float x, float y, float w, float h, float mx, float my);

static struct vpl_pin *
vpl_ide_hit_pins(struct vpl_node *node, float nx, float ny);

static int
vpl_ide_hit_pin(struct vpl_node *node,
                struct vpl_pin *pin, float nx, float ny);


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
  ide->num_wires    = 0;
  ide->alloc_wires  = VPL_ALLOC_WIRES;
  ide->wires = malloc(sizeof(struct vpl_wire)*VPL_ALLOC_WIRES);
}

void
vpl_ide_destroy(struct vpl_ide *ide) {
  free(ide->wires);
}


/**
 *  IDE Interaction
 *
 *  Handles mouse events in the IDE
 */
void
vpl_ide_interact(struct vpl_ide *ide, int m1, float mx, float my) {
  int m1_was_down = ide->interact_state & VPL_NSTATE_M1DOWN;

  // wasn't down before, now is
  if (!m1_was_down && m1 == 1) {
    vpl_ide_mousedown(ide, mx, my);
  }

  // was down before, now isn't
  if (m1_was_down && m1 == 0) {
    vpl_ide_mouseup(ide, mx, my);
  }

  if (ide->interact_state & VPL_NSTATE_M1DOWN)
  if ((ide->interact_state & VPL_NSTATE_PIN) == 0)
  if (ide->active_node != 0) {
    ide->active_node->x = mx - ide->active_hit_x;
    ide->active_node->y = my - ide->active_hit_y;
  }
}


/**
 *  Called after a mouseup event
 */
static inline void
vpl_ide_mouseup(struct vpl_ide *ide, float mx, float my) {

  struct vpl_node *hit_node;
  struct vpl_pin *hit_pin;
  struct vpl_pin *start_pin;

  start_pin = ide->active_pin;
  hit_node = vpl_ide_hit_nodes(ide, mx, my);

  if (hit_node) {
    // hit the pin where we dropped
    hit_pin = vpl_ide_hit_pins(hit_node, mx - hit_node->x, my - hit_node->y);

    // TODO: lang specific logic here
    if (hit_pin != start_pin)
    if (hit_pin)
    if (start_pin)
    if (ide->interact_state & VPL_NSTATE_PIN) {
      struct vpl_wire wire = { .start_pin = start_pin,
                               .end_pin   = hit_pin
                             };
      printf("adding wire %p %p\n", wire.start_pin, wire.end_pin);
      vpl_ide_add_wire(ide, wire);
    }
  }


  ide->interact_state &= ~(VPL_NSTATE_M1DOWN | VPL_NSTATE_PIN);
}


/**
 *  Called after a mousedown event
 */
static inline void
vpl_ide_mousedown(struct vpl_ide *ide, float mx, float my) {
  struct vpl_pin *hit_pin;
  static struct vpl_color hit_color = {128, 128, 128, 255};

  ide->interact_state |= VPL_NSTATE_M1DOWN;
  ide->active_node = vpl_ide_hit_nodes(ide, mx, my);

  if (ide->active_node) {
    ide->active_hit_x = mx - ide->active_node->x;
    ide->active_hit_y = my - ide->active_node->y;

    // TODO: pin hit detection
    hit_pin = vpl_ide_hit_pins(ide->active_node,
                               ide->active_hit_x,
                               ide->active_hit_y);

    if (hit_pin) {
      ide->active_pin = hit_pin;
      ide->interact_state |= VPL_NSTATE_PIN;
    }
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

static inline int
vpl_ide_hit_node(struct vpl_node *node, float mx, float my) {
  return vpl_ide_hit_box(node->x, node->y, node->w, node->h, mx, my);
}


/**
 *  IDE hit node
 *
 *  Check to see if a point is within a specific node
 */
static inline int
vpl_ide_hit_box(float x, float y, float w, float h, float mx, float my) {
  return mx >= x && mx <= x + w && my >= y && my <= y + h;
}


static inline int
vpl_ide_hit_pin(struct vpl_node *node,
                struct vpl_pin *pin, float nx, float ny) {
  return vpl_ide_hit_box(pin->x, pin->y, pin->size, pin->size, nx, ny);
}


static inline struct vpl_pin *
vpl_ide_hit_pins(struct vpl_node *node, float nx, float ny) {
  int i;
  struct vpl_pin *pin;

  for (i = 0; i < node->left_pin_count; ++i) {
    pin = &node->left_pins[i];
    if (vpl_ide_hit_pin(node, pin, nx, ny))
      return pin;
  }

  for (i = 0; i < node->right_pin_count; ++i) {
    pin = &node->right_pins[i];
    if (vpl_ide_hit_pin(node, pin, nx, ny))
      return pin;
  }

  for (i = 0; i < node->bottom_pin_count; ++i) {
    pin = &node->bottom_pins[i];
    if (vpl_ide_hit_pin(node, pin, nx, ny))
      return pin;
  }

  return NULL;
}
