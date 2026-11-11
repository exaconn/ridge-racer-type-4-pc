#ifndef RR_PSYQ_LIBGPU_GPUENV_H
#define RR_PSYQ_LIBGPU_GPUENV_H

#include "types.h"
#include "rr/gpu_environment_types.h"

/* PsyZ libgpu (__psyz). GpuRectangle is RECT, DrawEnvironment is DRAWENV,
 * DisplayEnvironment is DISPENV and GpuDmaTag is OT_TYPE/P_TAG; the layouts
 * are checked in src/main/psyq/sdk_abi.c. */
DrawEnvironment *SetDefDrawEnv(DrawEnvironment *environment, int x, int y, int width, int height);
DisplayEnvironment *SetDefDispEnv(DisplayEnvironment *environment, int x, int y, int width, int height);
int SetGraphDebug(int level);
/* PsyZ returns the previous callback as u_long. */
uintptr_t DrawSyncCallback(GpuCompletionCallback callback);
int DrawSync(int mode);
int ClearImage(GpuRectangle *rectangle, u8 red, u8 green, u8 blue);
int ClearImage2(GpuRectangle *rectangle, u8 red, u8 green, u8 blue);
int LoadImage(GpuRectangle *rectangle, const void *pixels);
int StoreImage(GpuRectangle *rectangle, void *pixels);
/* Ordering tables: entries are host packet tags. */
GpuDmaTag *ClearOTagR(GpuDmaTag *ordering, int count);
void DrawOTag(GpuDmaTag *ordering);
/* Builds the embedded DR_ENV (draw_packet1C), queues it and caches the
 * descriptor. */
DrawEnvironment *PutDrawEnv(DrawEnvironment *environment);
void DrawOTagEnv(GpuDmaTag *ordering, DrawEnvironment *environment);
DrawEnvironment *GetDrawEnv(DrawEnvironment *environment);

#endif /* RR_PSYQ_LIBGPU_GPUENV_H */
