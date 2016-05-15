
#include "prelude.h"
#include "node.h"
#include "ide.h"
#include <stdio.h>

#include <nanovg/nanovg.h>

static void
vpl_draw_pins(struct vpl_ide *vpl, struct vpl_node * node);

static void
vpl_draw_wire(struct vpl_ide *vpl, float x1, float y1, float x2, float y2);

static void
vpl_draw_pin_wire(struct vpl_ide *vpl,
                  struct vpl_node *node,
                  struct vpl_pin *pin,
                  float ex, float ey);

static NVGcolor
vpl_nvg_color(struct vpl_color col) {
  return nvgRGBAf(col.r, col.g, col.b, col.a);
}

void
vpl_node_draw(struct vpl_ide *vpl, struct vpl_node *node) {
  NVGpaint shadowPaint;
  NVGpaint headerPaint;

  float cornerRadius = node->roundness;
  NVGcontext *nvg = vpl->nvg;

  float x = node->x;
  float y = node->y;
  float w = node->w;
  float h = node->h;

  char *title = node->title;

  nvgSave(nvg);

  // Window
  nvgBeginPath(nvg);
  nvgRoundedRect(nvg, x,y, w,h, cornerRadius);
  nvgFillColor(nvg, nvgRGBA(28,30,34,192));
  nvgFill(nvg);

  // border
  nvgStrokeColor(nvg, vpl_nvg_color(node->border_color));
  nvgStroke(nvg);

  // Drop shadow
  shadowPaint = nvgBoxGradient(nvg, x,y+2, w,h, cornerRadius*2, 10,
                               nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
  nvgBeginPath(nvg);
  nvgRect(nvg, x-10,y-10, w+20,h+30);
  nvgRoundedRect(nvg, x,y, w,h, cornerRadius);
  nvgPathWinding(nvg, NVG_HOLE);
  nvgFillPaint(nvg, shadowPaint);
  nvgFill(nvg);

  // Header
  headerPaint = nvgLinearGradient(nvg, x,y,x,y+15, nvgRGBA(255,255,255,8),
                                  nvgRGBA(0,0,0,16));
  nvgBeginPath(nvg);
  nvgRoundedRect(nvg, x+1,y+1, w-2,30, cornerRadius-1);
  nvgFillPaint(nvg, headerPaint);
  nvgFill(nvg);
  nvgBeginPath(nvg);
  nvgMoveTo(nvg, x+0.5f, y+0.5f+30);
  nvgLineTo(nvg, x+0.5f+w-1, y+0.5f+30);
  nvgStrokeColor(nvg, nvgRGBA(0,0,0,32));
  nvgStroke(nvg);

  nvgFontSize(nvg, 18.0f);
  nvgFontFace(nvg, "sans-bold");
  nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

  /* nvgFontBlur(nvg,2); */
  nvgFillColor(nvg, nvgRGBA(0,0,0,128));
  nvgText(nvg, x+w/2,y+16+1, title, NULL);

  /* nvgFontBlur(nvg,0); */
  nvgFillColor(nvg, nvgRGBA(220,220,220,160));
  nvgText(nvg, x+w/2,y+16, title, NULL);

  nvgRestore(nvg);

  vpl_draw_pins(vpl, node);
}



void
vpl_ide_draw(struct vpl_ide *ide, struct vpl_node *nodes, int len, float mx, float my) {
  static int count = 0;
  struct vpl_node *node;

  for (int i = 0; i < len; i++) {
    node = &nodes[i];
    vpl_node_draw(ide, node);
  }

  // we have an active pin if we're dragging from it
  if (ide->interact_state & VPL_NSTATE_PIN)
  if (ide->active_node)
  if (ide->active_pin) {
    vpl_draw_pin_wire(ide, ide->active_node, ide->active_pin, mx, my);
  }

}

static void
pin_get_positioning(struct vpl_node *node,
                    struct vpl_pin *pins,
                    enum vpl_side pin_side,
                    int pin_count, int pin_ind,
                    float *px, float *py) {
  int i = 0;
  float x = 0;
  float y_offset = 40;
  float y = 0;
  float ty = 0;
  struct vpl_pin *pin = &pins[pin_ind];
  // TODO: pin alignment based off pin_count

  switch (pin_side) {
  case vpl_side_left: {
    for (i = 0, ty = 0; i < pin_ind; ++i) {
      ty += pins[i].size + node->pin_padding;
    }
    x = node->padding;
    y = node->padding + ty + y_offset;
    break;
  }
  case vpl_side_right: {
    for (i = 0, ty = 0; i < pin_ind; ++i) {
      ty += pins[i].size + node->pin_padding;
    }
    x = node->w - pin->size - node->padding;
    y = node->padding + ty + y_offset;
    break;
  }
  case vpl_side_bottom: {
    // TODO: position bottom pins
    break;
  }
  case vpl_side_top: {
    // TODO: position top pins, maybe under node label?
    break;
  }
  default:
    break;
  }

  *px = x;
  *py = y;
}



static void
pin_update_position(struct vpl_node *node,
                    struct vpl_pin *pins,
                    enum vpl_side pin_side,
                    int pin_count,
                    int pin_ind) {
  struct vpl_pin *pin = &pins[pin_ind];
  pin_get_positioning(node, pins, pin_side, pin_count, pin_ind,
                      &pin->x, &pin->y);
}



void
vpl_draw_pin(struct vpl_ide *vpl,
             struct vpl_node *node,
             struct vpl_pin *pin) {
  NVGcontext *nvg = vpl->nvg;
  float size = pin->size;

  nvgBeginPath(nvg);
  nvgRoundedRect(nvg, node->x + pin->x, node->y + pin->y, size, size, size);
  nvgFillColor(nvg, vpl_nvg_color(pin->color));
  nvgFill(nvg);
  nvgStrokeColor(nvg, vpl_nvg_color(pin->border_color));
  nvgStroke(nvg);
}



/**
 *  Draw pins on a node
 */
static void
vpl_draw_pins(struct vpl_ide *vpl, struct vpl_node * node) {
  int i = 0;

  // left pins
  for (i = 0; i < node->left_pin_count; i++) {
    // TODO: separate update step for pin positioning
    pin_update_position(node,
                        node->left_pins,
                        vpl_side_left,
                        node->left_pin_count,
                        i);

    vpl_draw_pin(vpl, node, &node->left_pins[i]);
  }

  // right pins
  for (i = 0; i < node->right_pin_count; i++) {
    // TODO: separate update step for pin positioning
    pin_update_position(node,
                        node->right_pins,
                        vpl_side_right,
                        node->right_pin_count,
                        i);

    vpl_draw_pin(vpl, node, &node->right_pins[i]);
  }

  // bottom pins
  for (i = 0; i < node->bottom_pin_count; i++) {
    // TODO: separate update step for pin positioning
    pin_update_position(node,
                        node->bottom_pins,
                        vpl_side_bottom,
                        node->bottom_pin_count,
                        i);

    vpl_draw_pin(vpl, node, &node->bottom_pins[i]);
  }
}


static void
vpl_draw_pin_wire(struct vpl_ide *ide,
                  struct vpl_node *node,
                  struct vpl_pin *pin,
                  float ex, float ey) {
  float x1 = node->x + pin->x + pin->size / 2;
  float y1 = node->y + pin->y + pin->size / 2;

  vpl_draw_wire(ide, x1, y1, ex, ey);
}


static void
vpl_draw_wire(struct vpl_ide *ide, float x1, float y1, float x2, float y2) {
  NVGcontext *nvg = ide->nvg;

  printf("p1 (%f, %f) p2 (%f, %f)\n", x1, y2, x2, y2);

	nvgBeginPath(nvg);
  /* nvgBezierTo(nvg, 10, 10, 20, 20, 30, 30); */

	nvgMoveTo(nvg, x1, y1);
  nvgQuadTo(nvg, x1*1.1, y1*1.1, x2, y2);
  /* nvgLineTo(nvg, 100, 100); */
  /* nvgLineTo(nvg, x2, y2); */
	nvgStrokeColor(nvg, nvgRGBA(255, 255, 255, 255));
	nvgStrokeWidth(nvg, 4.0f);
  nvgStroke(nvg);
	nvgStrokeWidth(nvg, 1.0f);
}
