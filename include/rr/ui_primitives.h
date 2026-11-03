#ifndef RR_UI_PRIMITIVES_H
#define RR_UI_PRIMITIVES_H

#include "rr/renderer_setup.h"
#include "rr/line_emit.h"

/* Semitransparent POLY_G4 rectangles. The two-color variant repeats the top
 * and bottom colors across each row; the four-color variant sets each corner. */
void func_8004CFB0(RrGpuRectangleFields *rectangle,
    RrGpuColorCommand *top, RrGpuColorCommand *bottom, s32 depth);
void func_8004D11C(RrGpuRectangleFields *rectangle,
    RrGpuColorCommand *topLeft, RrGpuColorCommand *topRight,
    RrGpuColorCommand *bottomLeft, RrGpuColorCommand *bottomRight, s32 depth);

#endif
