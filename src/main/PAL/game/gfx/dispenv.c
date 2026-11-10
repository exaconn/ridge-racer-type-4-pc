#include "psyq/libgte/gtemat5.h"
#include "types.h"
#include "scratchpad.h"
#include "game/gfx/dispenv.h"
#include "rr/overlay662_distance.h"
#include "rr/race_types.h"
#include "rr/track_lateral.h"
#include "game/data/cdfile2.h"

typedef struct {
    s32 f0;
    u8 r, g, b, pad;
} LightPreset8;

struct FogCameraContext48 {
    s32 unk00[7];
    int f1C, f20, f24, f28;
    s32 unk2C[3];
    int f38, f3C, f40, f44;
};

extern int GetTrackPointAhead();
extern struct FogCameraContext48 RaceContext;
/* same symbol under a 2nd C name: cse cannot reuse the earlier address, so the else arm re-materialises lui/addiu like the original */
extern struct FogCameraContext48 RaceContextAlias __asm__("RaceContext");
extern int CarTrackProgressDelta;
extern int CarTrackDirectionSign;
extern void RotMatrixZYX();
extern CamNode44 *TrackCameraNodes;
extern int SetSceneOverlayColor();
extern int SetSceneOverlayEnabled();
extern LightPreset8 *TrackLightPresets;

int ComputeCameraNodeDist(RaceVehicle *a0, CamNode44 *a1);
void GetTrackLaneWorldPos(RaceVehicle *a0, int *a1, int *a2);

int ComputeCameraNodeDist(RaceVehicle *a0, CamNode44 *a1)
{
    int d[3];
    int sp20[4];
    int s;
    struct FogCameraContext48 *q;
    int t;
    d[0] = (RaceContext.f20 / 4 - a1->x) / 4;
    d[1] = (RaceContext.f24 / 4 - a1->y) / 4;
    d[2] = (RaceContext.f28 / 4 - a1->z) / 4;
    s = CordicSqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) * 2;
    if (RaceContext.f1C != 0) {
        t = s >> 2;
        RaceContext.f3C = t;
        RaceContext.f38 = s - t;
        RaceContext.f44 = 0;
        RaceContext.f40 = s;
        if (s <= 0xFFFF || a0->half1EA != 0) {
            RaceContext.f44 = 1;
        }
        if (a1->node32 - a0->trackNodeF0 >= 0) CarTrackDirectionSign = 1;
    }
    t = s - RaceContext.f3C;
    if (RaceContext.f44 == 0) {
        if (t < 0) {
            RaceContext.f44 = 1;
        } else {
            q = &RaceContextAlias;
            if (q->f40 - s < 0) q->f44 = 1;
        }
    }
    GetTrackPointAhead(a0, sp20, a1->f14);
    return CarTrackProgressDelta = s;
}

void GetTrackLaneWorldPos(RaceVehicle *a0, int *a1, int *a2) {
    int d;
    int q2;
    int r;
    int t;
    int j;
    d = (a0->trackNodeF0 << 10) + a0->trackFraction170 - a2[2];
    t = d;
    if (d < 0) t = d + 0x3FF;
    r = t >> 10;
    q2 = r;
    r = d - (q2 << 10);
    d = q2;
    if (d < 0) d += TrackNodeCount;
    j = d + 1;
    if (TrackNodeCount < j) j -= TrackNodeCount;
    InterpolateLanePointXYZ(&TrackNodes[d], &TrackNodes[j], a2[0], r, a1);
    a1[0] = a1[0] << 2;
    a1[2] = a1[2] << 2;
    a1[1] = a1[1] + (a2[1] << 2);
}

void TransformCameraOffset(unsigned short *a, int b, int *c) {
    int m[8];
    unsigned short v[3];
    short r[3];
    int *s = (int *)SCRATCHPAD;
    s[6] = -(short)a[0];
    s[7] = (short)a[1];
    s[8] = (short)a[2];
    v[0] = a[0];
    v[1] = a[1];
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -b;
    RotMatrixZYX(v, m);
    ApplyMatrix((const MatrixWords32 *)(m), (const Vec3s8 *)(r), (Vec3i *)(c));
}

void RotateNegZByEulerZYX(unsigned short *a, int b, int *c) {
    int m[8];
    unsigned short v[3];
    short r[3];
    v[0] = a[0];
    v[1] = a[1];
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -b;
    RotMatrixZYX(v, m);
    ApplyMatrix((const MatrixWords32 *)(m), (const Vec3s8 *)(r), (Vec3i *)(c));
}

short LerpAngleWrap4096(CamNode44 *p, int t) {
    int n = p->fovDuration24 * 5;
    int v;
    if (n > 0 && t < n) {
        v = (p->fovEnd22 - p->fovStart20) * t / n + p->fovStart20;
    } else {
        v = p->fovEnd22;
    }
    return v - (v / 4096 * 4096);
}

s32 CountTrackEntries(void) {
    s32 var_a1;
    CamNode44 *var_a0;

    var_a0 = TrackCameraNodes;
    var_a1 = 0;
    if (var_a0->node32 != -1) {
        do {
            var_a0++;
            var_a1 += 1;
        } while (var_a0->node32 != -1);
    }
    return var_a1;
}

void ApplyTrackLightPreset(int a0) {
    int s2, s0, x, v1, y;
    LightPreset8 *s1;
    s1 = TrackLightPresets;
    s2 = 0;
    s0 = -1;
    a0 = ((unsigned)a0 >> 4) & 0xFF;
    /* ASM_BARRIER: Keep shifted argument in a separate register across modulo and threshold checks. */
    __asm__("" : "=r"(x) : "0"(a0));
    if (a0 < 8) {
        s0 = (a0 - 1) % 4;
    }
    if (x >= 5) {
        s2 = 1;
    }
    if (s0 >= 0) {
        SetSceneOverlayEnabled(1, x);
        s1 += s0;
        SetSceneOverlayColor(s1->f0, s1->r, s1->g, s1->b);
        v1 = 7;
    } else {
        SetSceneOverlayEnabled(0, x);
        v1 = 7;
    }
    for (v1 = 7; v1 >= 0; v1--) {
        CarSlotBuffers[v1].overlay70 = s2;
    }
}
