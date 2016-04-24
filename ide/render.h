#ifndef VPL_RENDER_H
#define VPL_RENDER_H

#include "ide.h"
#include "node.h"

void
vpl_node_draw(struct vpl_ide *vpl, struct vpl_node *node);

void
vpl_ide_draw(struct vpl_ide *vpl, struct vpl_node *nodes, int len);

#endif /* VPL_RENDER_H */
