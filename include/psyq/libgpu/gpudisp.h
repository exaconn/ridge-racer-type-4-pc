#ifndef RR_PSYQ_LIBGPU_GPUDISP_H
#define RR_PSYQ_LIBGPU_GPUDISP_H

#include "rr/gpu_environment_types.h"

/* Horizontal timing for the two video standards and five width classes. */
typedef struct {u16 start,end;} GpuHorizontalRange;
extern GpuHorizontalRange DispHRangeTable[2][5];
extern u8 DispDotClockDivisors[5];
extern const char StrPutDispEnvFmt[];

/* Callbacks may change the descriptor, cached fields, timing tables and live
 * driver. Video-mode results used as table indices must be0 or1. Input and
 * cached descriptors may be identical. The final20-byte copy is read live. */
DisplayEnvironment *PutDispEnv(DisplayEnvironment *environment);
DisplayEnvironment *GetDispEnv(DisplayEnvironment *environment);

RR_ASSERT_SIZE(GpuHorizontalRange,4);

void SetDrawOffset(void *arg0, void *arg1);
/* Three-word DR_STP command backing, including the zero second command. */
void SetDrawStp(GpuDrawModePacket *packet, int enabled);

#endif /* RR_PSYQ_LIBGPU_GPUDISP_H */
