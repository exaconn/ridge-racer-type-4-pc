#ifndef RR_PSYQ_LIBGPU_GPUDISP_H
#define RR_PSYQ_LIBGPU_GPUDISP_H

#include "rr/gpu_environment_types.h"

/* PsyZ libgpu (__psyz) display and state-packet builders. State packets
 * (DR_MODE/DR_AREA/DR_OFFSET/DR_TWIN/DR_STP) are a host tag followed by two
 * u_long command cells: GpuDrawModePacket (packet arguments are void * so
 * scratch cursors can be passed directly). SetDrawEnv targets a
 * GpuDrawEnvironmentPacket. */
DisplayEnvironment *PutDispEnv(DisplayEnvironment *environment);
DisplayEnvironment *GetDispEnv(DisplayEnvironment *environment);

void SetDrawArea(void *packet, GpuRectangleFields *rectangle);
/* PsyZ reads ofs[0], ofs[1] (u_short *): pass &rectangle->x. */
void SetDrawOffset(void *packet, GpuRectangleFields *offset);
void SetTexWindow(void *packet, GpuRectangleFields *window);
/* NULL window emits texture-window command zero. */
void SetDrawMode(void *packet, int draw_display, int dither, int page,
    GpuRectangleFields *window);
/* PsyZ declares but does not implement SetDrawStp; the game copy lives in
 * src/main/psyq/libgpu/gpudisp.c. */
void SetDrawStp(GpuDrawModePacket *packet, int enabled);

/* Fills the DR_ENV (one u_long cell per command) for environment. */
void SetDrawEnv(void *packet, DrawEnvironment *environment);
void SetDrawEnv2(void *packet, DrawEnvironment *environment);

#endif /* RR_PSYQ_LIBGPU_GPUDISP_H */
