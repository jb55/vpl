
#include "wire.h"
#include "ide.h"
#include "vec/vec.h"

#include <stdlib.h>

void vpl_ide_add_wire(struct vpl_ide *ide, struct vpl_wire wire) {
  vec_push(&ide->vec_wires, wire);
}

void vpl_ide_remove_wire(struct vpl_ide *ide, struct vpl_wire *wire) {
  *wire = ide->vec_wires.data[ide->vec_wires.length--];
}
