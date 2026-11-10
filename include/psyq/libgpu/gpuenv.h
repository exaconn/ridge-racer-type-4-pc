#ifndef RR_PSYQ_LIBGPU_GPUENV_H
#define RR_PSYQ_LIBGPU_GPUENV_H

#include "types.h"
#include "rr/gpu_environment_types.h"

extern char StrStoreImage[];
extern char MoveImageNameStr[];
extern const char D_80011F38[];
DrawEnvironment *SetDefDrawEnv(DrawEnvironment *environment, int x, int y, int width, int height);
DisplayEnvironment *SetDefDispEnv(DisplayEnvironment *p, int a, int b, int c, int d);
s32 SetGraphDebug(s32 enabled);
/* Replaces the completion callback and returns the previous registration.
 * The debug logger runs before the previous registration is captured.
 * A null registration disables notification. The queue invokes the callback
 * after restoring interrupts, when it is empty and DMA is idle; the pending
 * flag is cleared before entry, so user code may register/rearm another pass. */
GpuCompletionCallback DrawSyncCallback(GpuCompletionCallback callback);
void ClearImage(GpuRectangle *rectangle, s32 red, s32 green, s32 blue);
void ClearImage2(GpuRectangle *rectangle, s32 red, s32 green, s32 blue);
void LoadImage(GpuRectangle *rectangle, const void *pixels);
void StoreImage(GpuRectangle *rectangle, void *pixels);
unsigned *ClearOTagR(unsigned *a, int b);
/* Submission builds the embedded packet, queues64 bytes, then copies92 bytes
 * into the cached draw descriptor. Helpers may mutate the live descriptor. */
DrawEnvironment *PutDrawEnv(DrawEnvironment *environment);
/* Ordering is a CPU pointer encoded only when it becomes the packet link.
 * Native ordering buffers must have a registered DMA address. */
void DrawOTagEnv(u32 *ordering, DrawEnvironment *environment);
DrawEnvironment *GetDrawEnv(DrawEnvironment *environment);

#endif /* RR_PSYQ_LIBGPU_GPUENV_H */
