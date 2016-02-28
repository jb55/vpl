#ifndef VPL_RENDER_H
#define VPL_RENDER_H

#include "ide.h"
#include "node.h"

void
vpl_draw_node(struct vpl_ctx *vpl, struct vpl_node *node);

void
vpl_draw_ide(struct vpl_ctx *vpl, struct vpl_node *nodes, int len);

#endif /* VPL_RENDER_H */
