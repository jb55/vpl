
int blowup = 0;
int screenshot = 0;
int premult = 0;

void
vpl_ide_interact(struct vpl_ide *ide, int m1,
                 struct vpl_action *action) {
  if (ide->m1 == 0 && m1 == 1) {
    vpl_ide_mousedown(ide);
  }

  if (m1 == 0) {
    vpl_ide_mouseup(ide);
  }
}



void
vpl_ide_mouseup(struct vpl_ide *ide) {
  ide->m1 = 0;
  ide->interact_state = VPL_NSTATE_IDLE;
}


void
vpl_ide_mousedown(struct vpl_ide *ide) {
  ide->m1 = 1;
}
