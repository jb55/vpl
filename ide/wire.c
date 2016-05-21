
#include "wire.h"
#include "ide.h"

#include <stdlib.h>

void vpl_ide_add_wire(struct vpl_ide *ide, struct vpl_wire wire) {
  int num_wires = ide->num_wires++;
  if (num_wires > ide->alloc_wires) {
    ide->alloc_wires *= 1.5;
    ide->wires = realloc(ide->wires, ide->alloc_wires * sizeof(struct vpl_wire));
  }
  ide->wires[num_wires] = wire;
}

void vpl_ide_remove_wire(struct vpl_ide *ide, struct vpl_wire *wire) {
  *wire = ide->wires[ide->num_wires--];
}
