#ifndef RR_FRAME_LOOP_H
#define RR_FRAME_LOOP_H
#include "rr/frame_environment.h"

#include "rr/frame_storage_types.h"
typedef void (*FrameStep)(void);
extern FrameStorage D_800ADAF0[2];
extern FrameStep *ModeFuncTable[];
extern u32 FrameCounter;
extern s32 VSyncThreshold, DrawSubOtFlag, ModeFrame, DrawBufIndex;
extern u16 ModeGroup, D_800F3A26;
extern s16 D_800F4D8A;

/* Initializes once, then runs indefinitely. Helpers may replace the current
 * context, frame counter, dispatch selectors and frame-wait threshold.
 * The threshold is captured once per frame after DrawSync. All later context
 * arguments are read afresh. Dispatch selectors require accessible table rows. */
void MainLoopStartupHook(void);
void MainLoop(void);
#endif
