#include "m2c_macros.h"

/* legacy typing of shared globals in this part of the unit */
#define RaceContext (*(Context650 (*))&RaceContext)

#undef RaceContext

/* legacy typing of shared globals in this part of the unit */
#define RaceContext (*(int (*)[])&RaceContext)

#define SecondaryViewportCam (*(char (*)[])&SecondaryViewportCam)

#define SceneRenderScratchPad (*(int (*))&SceneRenderScratchPad)

#define CurrentDrawBuf ((char *)CurrentDrawBuf)

#undef RaceContext

#undef SecondaryViewportCam

#undef SceneRenderScratchPad

#undef CurrentDrawBuf

extern int PsyqBzero();

extern M2C_UNK SceneRenderScratchPad;

void ResetLensFlareState(void) {
    PsyqBzero(&SceneRenderScratchPad, 0x20);
    M2C_FIELD(&SceneRenderScratchPad, s32 *, 0x1C) = -1;
}
