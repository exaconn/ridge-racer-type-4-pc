#ifndef RR_GAME_GFX_DISPENV_H
#define RR_GAME_GFX_DISPENV_H

#include "rr/layout.h"

typedef struct CamNode44 {
    int x, y, z, f0C;
    int f10, f14, f18, f1C;
    s16 fovStart20, fovEnd22, fovDuration24, unk26;
    int f28, f2C;
    u16 f30;
    s16 node32;
    s32 unk34[4];
} CamNode44;

/* Palette entries carry a full blend mode and three serialized color bytes. */
typedef struct TrackLightPreset8 {
    s32 mode;
    u8 r, g, b, reserved;
} TrackLightPreset8;
RR_ASSERT_SIZE(TrackLightPreset8, 8);
RR_ASSERT_OFFSET(TrackLightPreset8, r, 4);
struct RaceVehicle;
int ComputeCameraNodeDist(struct RaceVehicle *vehicle, CamNode44 *node);
void GetTrackLaneWorldPos();
void TransformCameraOffset(unsigned short *a, int b, int *c);
void RotateNegZByEulerZYX(unsigned short *a, int b, int *c);
short LerpAngleWrap4096(CamNode44 *p, int t);
s32 CountTrackEntries(void);
void ApplyTrackLightPreset(int encodedIndex);

#endif /* RR_GAME_GFX_DISPENV_H */
