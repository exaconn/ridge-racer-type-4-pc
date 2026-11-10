#ifndef RR_GAME_GFX_POLY1_H
#define RR_GAME_GFX_POLY1_H

#include "types.h"
#include "rr/animated_track_types.h"

/* Workspace is a complete AnimatedTrackArena, followed by serialized color keys. */
void InitAnimatedTrackObjects(void *memory);
extern AnimatedTrackParticle (*TrackParticlePoolPtr)[255];
extern const AnimatedTrackColorKey *TrackColorKeysPrimary[9],*TrackColorKeysSecondary[9];
extern AnimatedTrackObject *AnimatedTrackObjPool;
void SetupAnimatedObjectMatrices(AnimatedTrackObject *object);
void SetAnimatedTrackObjectState(int index, int flags, const s32 *position, int style);
void DrawAnimatedTrackObject(AnimatedTrackScratch *scratch, int index, int advance);
s32 GetSegmentVisibilityTableSize(void);
void ApplySegmentVisibilityMask(void);
void SetSegmentVisibilityTable(s32 arg0);
s32 GetOctantLookupTableSize(void);
int LookupOctantVisibility(void);
void SetOctantLookupTable(s32 arg0);
void InitSplashDispEnv(void);
void RelocateCountedOffsetTable(int *a0);
void InitRearViewMirrorState(void);
int SetupRearViewMirror(void);
void RestoreMainViewAfterMirror(void);
void UpdateAndDrawRearViewMirror(int arg0);


#endif /* RR_GAME_GFX_POLY1_H */
