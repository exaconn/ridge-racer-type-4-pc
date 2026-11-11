#ifndef RR_GAME_GFX_POLY1_H
#define RR_GAME_GFX_POLY1_H

#include "types.h"
#include "rr/animated_track_types.h"

/* memory is the course 7 region that reserves the PS1 pools; its color key
 * table follows them (RR_ANIMATED_TRACK_WIRE_KEYS). */
void InitAnimatedTrackObjects(void *memory);
extern AnimatedTrackParticle (*TrackParticlePoolPtr)[255];
extern const AnimatedTrackColorKey *TrackColorKeysPrimary[9],*TrackColorKeysSecondary[9];
extern AnimatedTrackObject *AnimatedTrackObjPool;
void SetupAnimatedObjectMatrices(AnimatedTrackObject *object);
void SetAnimatedTrackObjectState(int index, int flags, const s32 *position, int style);
void DrawAnimatedTrackObject(AnimatedTrackScratch *scratch, int index, int advance);
s32 GetSegmentVisibilityTableSize(void);
void ApplySegmentVisibilityMask(void);
void SetSegmentVisibilityTable(short *arg0);
s32 GetOctantLookupTableSize(void);
int LookupOctantVisibility(void);
void SetOctantLookupTable(unsigned char *arg0);
void InitSplashDispEnv(void);
void RelocateCountedOffsetTable(int *a0);
void InitRearViewMirrorState(void);
int SetupRearViewMirror(void);
void RestoreMainViewAfterMirror(void);
void UpdateAndDrawRearViewMirror(int arg0);


#endif /* RR_GAME_GFX_POLY1_H */
