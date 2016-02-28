
#include "prelude.h"
#include "node.h"
#include "ide.h"

#include <nanovg/nanovg.h>

void
vpl_draw_node(struct vpl_ctx *vpl, struct vpl_node *node) {
	NVGpaint shadowPaint;
	NVGpaint headerPaint;

	float cornerRadius = node->roundness;
	NVGcontext *vg = vpl->nvg;

	float x = node->x;
	float y = node->y;
	float w = node->w;
	float h = node->h;

	char *title = node->title;

	nvgSave(vg);

	// Window
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28,30,34,192));
//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);

	// Drop shadow
	shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10,
                               nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	nvgBeginPath(vg);
	nvgRect(vg, x-10,y-10, w+20,h+30);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	// Header
	headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8),
                                  nvgRGBA(0,0,0,16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
	nvgFillPaint(vg, headerPaint);
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgMoveTo(vg, x+0.5f, y+0.5f+30);
	nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
	nvgStroke(vg);

	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

	/* nvgFontBlur(vg,2); */
	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgText(vg, x+w/2,y+16+1, title, NULL);

	/* nvgFontBlur(vg,0); */
	nvgFillColor(vg, nvgRGBA(220,220,220,160));
	nvgText(vg, x+w/2,y+16, title, NULL);

	nvgRestore(vg);
}



void
vpl_draw_ide(struct vpl_ctx *vpl, struct vpl_node *nodes, int len) {
  struct vpl_node *node;
  for (int i = 0; i < len; i++) {
    node = &nodes[i];
    vpl_draw_node(vpl, node);
  }
}


static void
pin_get_positioning(struct vpl_node *node,
                    struct vpl_pin *pins,
                    int npins, int pin_ind,
                    float *px, float *py) {
  float x = 0;
  float y = 0;

  /* switch (pin->side) { */
  /* case vpl_side_left: { */
  /*   x = node->x + node->padding; */
  /*   x = node->x + node->padding; */
  /*   break; */
  /* } */
  /* case vpl_side_right: { */
  /*   x = */
  /*   break; */
  /* } */
  /* case vpl_side_bottom: { */
  /*   break; */
  /* } */
  /* case vpl_side_top: { */
  /*   break; */
  /* } */
  /* default: */
  /*   break; */
  /* } */

  *px = x;
  *py = y;
}


void
vpl_draw_pin(struct vpl_ctx *vpl,
             struct vpl_node *node,
             struct vpl_pin *pins,
             int npins, int pin_ind) {
  NVGcontext *vg = vpl->nvg;
  struct vpl_pin *pin = &pins[pin_ind];
  float x = 0;
  float y = 0;
  float size = pin->size;

  pin_get_positioning(node, pins, npins, pin_ind, &x, &y);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, size, size, size);
	nvgFillColor(vg, nvgRGBA(pin->color.r, pin->color.g, pin->color.b,
                           pin->color.a));
	nvgFill(vg);
}


void
vpl_draw_pins(struct vpl_ctx *vpl, struct vpl_node * node) {
  float i = 0;

  // left pins
  for (i = 0; i < node->left_pin_count; i++) {
    vpl_draw_pin(vpl, node, node->left_pins, node->left_pin_count, i);
  }

  // right pins
  for (i = 0; i < node->right_pin_count; i++) {
    vpl_draw_pin(vpl, node, node->right_pins, node->right_pin_count, i);
  }

  // bottom pins
  for (i = 0; i < node->bottom_pin_count; i++) {
    vpl_draw_pin(vpl, node, node->bottom_pins, node->bottom_pin_count, i);
  }
}
