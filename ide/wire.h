
#ifndef WIRE_H
#define WIRE_H

#include "color.h"

struct vpl_wire
{
  struct vpl_pin *start_pin;
  struct vpl_pin *end_pin;
  struct vpl_color color;
};



#endif /* WIRE_H */
