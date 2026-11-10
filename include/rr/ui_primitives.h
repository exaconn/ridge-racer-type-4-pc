#ifndef RR_UI_PRIMITIVES_H
#define RR_UI_PRIMITIVES_H

#include "rr/renderer_setup.h"
#include "rr/line_emit.h"

/* Semitransparent POLY_G4 rectangles. The two-color variant repeats the top
 * and bottom colors across each row; the four-color variant sets each corner. */
void AddVerticalGradientRect(GpuRectangleFields *rectangle,
    GpuColorCommand *top, GpuColorCommand *bottom, s32 depth);
void AddGouraudRect(GpuRectangleFields *rectangle,
    GpuColorCommand *topLeft, GpuColorCommand *topRight,
    GpuColorCommand *bottomLeft, GpuColorCommand *bottomRight, s32 depth);

#endif
