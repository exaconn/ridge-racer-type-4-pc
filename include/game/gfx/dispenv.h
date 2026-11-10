#ifndef RR_GAME_GFX_DISPENV_H
#define RR_GAME_GFX_DISPENV_H

#include "types.h"

typedef struct {
    int x, y, z, f0C;
    int f10, f14, f18, f1C;
    s16 fovStart20, fovEnd22, fovDuration24, unk26;
    int f28, f2C;
    u16 f30;
    s16 node32;
    s32 unk34[4];
} CamNode44;

int ComputeCameraNodeDist();
void GetTrackLaneWorldPos();
void TransformCameraOffset(unsigned short *a, int b, int *c);
void RotateNegZByEulerZYX(unsigned short *a, int b, int *c);
short LerpAngleWrap4096(CamNode44 *p, int t);
s32 CountTrackEntries(void);
void ApplyTrackLightPreset(int a0);

#endif /* RR_GAME_GFX_DISPENV_H */
