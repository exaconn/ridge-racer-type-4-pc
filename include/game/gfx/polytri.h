#ifndef RR_GAME_GFX_POLYTRI_H
#define RR_GAME_GFX_POLYTRI_H

#include "types.h"
#include "rr/layout.h"

typedef struct {
    s16 vx, vy, vz, reserved06;
} SemiTransSVec;

typedef struct {
    u8 r, g, b, cd;
} SemiTransCVec;

typedef struct {
    SemiTransSVec v0;
    SemiTransSVec v1;
    SemiTransSVec v2;
    SemiTransSVec reserved_v[2];
    u8 reserved_28[4];
    SemiTransCVec c0;
    SemiTransCVec c1;
    SemiTransCVec c2;
} SemiTransTri38;

typedef struct {
    SemiTransSVec v0;
    SemiTransSVec v1;
    SemiTransSVec v2;
    SemiTransSVec v3;
    SemiTransSVec reserved_v[3];
    SemiTransCVec c0;
    SemiTransCVec c1;
    SemiTransCVec c2;
    SemiTransCVec c3;
} SemiTransQuad48;

RR_ASSERT_SIZE(SemiTransTri38, 0x38);
RR_ASSERT_OFFSET(SemiTransTri38, c0, 0x2C);
RR_ASSERT_SIZE(SemiTransQuad48, 0x48);
RR_ASSERT_OFFSET(SemiTransQuad48, c0, 0x38);

void EmitSemiTransG3List(SemiTransTri38 *a0, s32 a1, s32 a2);
void EmitSemiTransG4List(SemiTransQuad48 *a0, s32 a1, s32 a2);
void UpdateTrackObjectActivation(void);

#endif /* RR_GAME_GFX_POLYTRI_H */
