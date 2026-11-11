#ifndef RR_FRAME_LOOP_H
#define RR_FRAME_LOOP_H
#include "rr/frame_environment.h"

#include "rr/frame_storage_types.h"
typedef void (*FrameStep)(void);
extern FrameStorage DrawBuffers[2];
extern FrameStep *ModeFuncTable[];
extern u32 FrameCounter;
extern s32 VSyncThreshold, DrawSubOtFlag, ModeFrame, DrawBufIndex;
extern u16 ModeGroup, ModeIndex;
extern s16 SubmodeState;

/* Initializes once, then runs indefinitely. Helpers may replace the current
 * context, frame counter, dispatch selectors and frame-wait threshold.
 * The threshold is captured once per frame after DrawSync. All later context
 * arguments are read afresh. Dispatch selectors require accessible table rows. */
void MainLoopStartupHook(void);

/* Host frame pacing. On the PS1 a frame that renders the race world (any
 * SetupPlayerView: races, intro, replays, attract demo, split screen) takes
 * longer than one field, so VSync(0) returns on the second blank and races
 * run at 25 Hz (PAL); lap timing counts 40 ms per frame. The host does the
 * work well inside a field, so SetupPlayerView raises this to 2 and MainLoop
 * waits that many fields since the previous frame (VSync(n)); 1 otherwise. */
extern s32 HostFrameFields;
void MainLoop(void);
#endif
