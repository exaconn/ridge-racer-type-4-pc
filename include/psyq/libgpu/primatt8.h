#ifndef RR_PSYQ_LIBGPU_PRIMATT8_H
#define RR_PSYQ_LIBGPU_PRIMATT8_H

#include "types.h"
#include "rr/gpu.h"

/* PsyZ: void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y); DR_MOVE is a
 * host tag plus five u_long cells. */
void SetDrawMove(void *packet, GpuRectangleFields *source, int x, int y);

#endif /* RR_PSYQ_LIBGPU_PRIMATT8_H */
