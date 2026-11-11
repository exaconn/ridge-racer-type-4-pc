#include "rr/scene_camera_state.h"
#include "game/course/trkintp2.h"
#include "game/gfx/scene1.h"
#include "psyq/libgte/gtemat5.h"
#include "types.h"
#include "scratchpad.h"
#include "game/gfx/dispenv.h"
#include "psyq/libgte/gtecord.h"
#include "rr/race_types.h"
#include "rr/track_lateral.h"
#include "game/data/cdfile2.h"

extern SceneRaceContext RaceContextAlias __asm__("RaceContext");
extern void RotMatrixZYX();

void GetTrackLaneWorldPos(RaceVehicle *a0, int *a1, int *a2);

int ComputeCameraNodeDist(RaceVehicle *vehicle, CamNode44 *node)
{
    Vec3i delta;
    int pointAhead[3];
    int distance;
    SceneRaceContext *context;
    int remaining;
    delta.x = (s32)((u32)(RaceContext.cameraFollowPosition20.vector.x / 4) - node->x) / 4;
    delta.y = (s32)((u32)(RaceContext.cameraFollowPosition20.vector.y / 4) - node->y) / 4;
    delta.z = (s32)((u32)(RaceContext.cameraFollowPosition20.vector.z / 4) - node->z) / 4;
    distance = (s32)((u32)CordicSqrt((s32)((u32)delta.x * delta.x + (u32)delta.y * delta.y + (u32)delta.z * delta.z)) * 2U);
    if (RaceContext.cameraNodeChanged1C != 0) {
        remaining = distance >> 2;
        RaceContext.cameraNearDistance3C = remaining;
        RaceContext.cameraDistanceSpan38 = (s32)((u32)distance - remaining);
        RaceContext.cameraLimitReached44 = 0;
        RaceContext.cameraPreviousDistance40 = distance;
        if (distance <= 0xFFFF || vehicle->half1EA != 0) {
            RaceContext.cameraLimitReached44 = 1;
        }
        if ((s32)((u32)node->node32 - vehicle->trackNodeF0) >= 0) RaceContext.cameraLimitReached44 = 1;
    }
    remaining = (s32)((u32)distance - RaceContext.cameraNearDistance3C);
    if (RaceContext.cameraLimitReached44 == 0) {
        if (remaining < 0) {
            RaceContext.cameraLimitReached44 = 1;
        } else {
            context = &RaceContextAlias;
            if ((s32)((u32)context->cameraPreviousDistance40 - distance) < 0) context->cameraLimitReached44 = 1;
        }
    }
    GetTrackPointAhead(vehicle, pointAhead, node->f14);
    return RaceContext.cameraPreviousDistance40 = distance;
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

    var_a0 = RaceContext.cameraNodes550;
    var_a1 = 0;
    if (var_a0->node32 != -1) {
        do {
            var_a0++;
            var_a1 += 1;
        } while (var_a0->node32 != -1);
    }
    return var_a1;
}

void ApplyTrackLightPreset(int encodedIndex) {
    int applyToSlots, presetIndex, slot;
    s16 selector;
    TrackLightPreset8 *preset;
    preset = RaceContext.lightPresets558;
    applyToSlots = 0;
    presetIndex = -1;
    selector = ((u32)encodedIndex >> 4) & 0xFF;
    if (selector < 8) {
        presetIndex = (selector - 1) % 4;
    }
    if (selector >= 5) {
        applyToSlots = 1;
    }
    if (presetIndex >= 0) {
        SetSceneOverlayEnabled(1);
        preset += presetIndex;
        SetSceneOverlayColor(preset->mode, preset->r, preset->g, preset->b);
    } else {
        SetSceneOverlayEnabled(0);
    }
    for (slot = 7; slot >= 0; slot--) {
        CarSlotBuffers[slot].overlay70 = applyToSlots;
    }
}
