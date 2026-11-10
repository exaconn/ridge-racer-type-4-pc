#include "psyq/libgte/rotmat.h"
#include "psyq/libgte/gtemat5.h"
#include "rr/frame_storage_types.h"
#include "game/car/car.h"
#include "scratchpad.h"
#include "rr/scene_camera_state.h"
#include "rr/player_cars.h"
#include "rr/projection_state.h"
#include "rr/gpu.h"
#include "rr/line_emit.h"
#include "game/camera/camfx.h"
#include "game/gfx/gfxotag.h"
#include "game/effect/spark.h"
#include "game/effect/carfx.h"
#include "game/math/rotmat.h"
#include "psyq/libc/rand.h"

extern int SetGeomScreen();
extern VectorWords16 D_800A0FC0;
extern Vec3s8 D_800A0FD0;
extern int D_800A0FD8;
extern unsigned char D_800A0DC8[][4];
extern u16 SparkSpriteTexpage;
extern short *D_800A0F20[];
extern short *D_800A0F8C[];
extern short SelectedTeamIndex;

int StepCarShowcasePhysics(RaceVehicle *arg0, int arg1, int arg2, int *arg3);
void DrawHudSprite(int a0, int a1, int a2, int a3, int a4);
void DrawHudDigit();
void UpdateSparkBurstEffectA(int a0, int a1);

/* Interpolate the low32-bit displacement, then divide it as a signed word.
 * Callers add the base in unsigned arithmetic, preserving MIPS word wrap. */
static inline u32 InterpolateCameraDelta(u32 end, u32 start, u32 elapsed, s32 duration) {
    s32 numerator = (end - start) * elapsed;
    return numerator / duration;
}

int StepCarShowcasePhysics(RaceVehicle *vehicle, int time, int track, int *segmentOut) {
    VectorWords16 sp10;
    VectorWords16 startPosition;
    VectorWords16 endPosition;
    MatrixWords32 rotation;
    Vec3s8 orbitAngles;
    Vec3s8 orbitOffset;
    VectorWords16 orbitPosition;
    ProjectionScratchPrefix *projection = (ProjectionScratchPrefix *)SCRATCHPAD;
    int endKey = -1;
    int startKey;
    int i;
    SceneCameraKeyframe2C *keys;
    int duration;
    int elapsed;

    keys = RaceContext.camera24C->tracks138[track];
    for (i = 1; i < 0x18; i++) {
        s32 keyTime = keys[i].time24 * 10U;
        if (keyTime >= time) {
            endKey = i;
            startKey = i - 1;
            *segmentOut = startKey;
            break;
        }
    }

    if (endKey < 0) {
        orbitAngles.fields.x = 0;
        orbitAngles.fields.y = -0xA00;
        orbitAngles.fields.z = 0;
        orbitOffset.fields.x = D_800A0FC0.xyz.x;
        orbitOffset.fields.y = D_800A0FC0.xyz.y;
        orbitOffset.fields.z = D_800A0FC0.xyz.z;
        RotMatrix(&orbitAngles.fields, &rotation);
        ApplyMatrix(&rotation, &orbitOffset, &orbitPosition.xyz);
        orbitPosition.xyz.x += PlayerCars[0].position20.vector.x * 4U;
        orbitPosition.xyz.y += PlayerCars[0].position20.vector.y * 4U;
        orbitPosition.xyz.z += PlayerCars[0].position20.vector.z * 4U;
        projection->pose08.position00.xyz.x = orbitPosition.xyz.x;
        projection->pose08.position00.xyz.y = orbitPosition.xyz.y;
        projection->pose08.position00.xyz.z = orbitPosition.xyz.z;
        projection->pose08.angles10.xyz.x = D_800A0FD0.fields.x;
        projection->pose08.angles10.xyz.y = D_800A0FD0.fields.y;
        projection->pose08.angles10.xyz.z = D_800A0FD0.fields.z;
        SetGeomScreen(D_800A0FD8);
        BuildCameraMatrices();
        return 1;
    }

    if (startKey != RaceContext.camera24C->segment10C) {
        RaceContext.camera24C->segment10C = startKey;
        if (keys[startKey].restart28 != 0) {
            RaceContext.camera24C->phase108 = keys[startKey].restart28 * 10U;
        }
    }

    UpdateCarShowcaseCamera(RaceContext.camera24C->phase108);

    duration = (keys[endKey].time24 - (u32)keys[startKey].time24) * 10U;
    elapsed = time - keys[startKey].time24 * 10U;

    if (keys[startKey].view00.absolute18 == 0) {
        startPosition.xyz.x = vehicle->position20.vector.x * 4U;
        startPosition.xyz.y = vehicle->position20.vector.y * 4U;
        startPosition.xyz.z = vehicle->position20.vector.z * 4U;
    } else {
        startPosition = keys[startKey].view00.pose00.position00;
    }

    if (keys[endKey].view00.absolute18 == 0) {
        endPosition.xyz.x = vehicle->position20.vector.x * 4U;
        endPosition.xyz.y = vehicle->position20.vector.y * 4U;
        endPosition.xyz.z = vehicle->position20.vector.z * 4U;
    } else {
        endPosition = keys[endKey].view00.pose00.position00;
    }

    if (keys[endKey].time24 < 0x50) {
        if (duration > 0) {
            RaceContext.view60.pose00.position00.xyz.x = InterpolateCameraDelta(endPosition.xyz.x, startPosition.xyz.x, elapsed, duration) + startPosition.xyz.x;
            RaceContext.view60.pose00.position00.xyz.y = InterpolateCameraDelta(endPosition.xyz.y, startPosition.xyz.y, elapsed, duration) + startPosition.xyz.y;
            RaceContext.view60.pose00.position00.xyz.z = InterpolateCameraDelta(endPosition.xyz.z, startPosition.xyz.z, elapsed, duration) + startPosition.xyz.z;
            RaceContext.view60.pose00.rotation10.x = InterpolateCameraDelta(keys[endKey].view00.pose00.rotation10.x, keys[startKey].view00.pose00.rotation10.x, elapsed, duration) + keys[startKey].view00.pose00.rotation10.x;
            RaceContext.view60.pose00.rotation10.y = InterpolateCameraDelta(keys[endKey].view00.pose00.rotation10.y, keys[startKey].view00.pose00.rotation10.y, elapsed, duration) + keys[startKey].view00.pose00.rotation10.y;
            RaceContext.view60.pose00.rotation10.z = InterpolateCameraDelta(keys[endKey].view00.pose00.rotation10.z, keys[startKey].view00.pose00.rotation10.z, elapsed, duration) + keys[startKey].view00.pose00.rotation10.z;
            RaceContext.view60.distance1C = InterpolateCameraDelta(keys[endKey].view00.distance1C, keys[startKey].view00.distance1C, elapsed, duration) + keys[startKey].view00.distance1C;
            RaceContext.view60.projection20 = InterpolateCameraDelta(keys[endKey].view00.projection20, keys[startKey].view00.projection20, elapsed, duration) + keys[startKey].view00.projection20;
        } else {
            RaceContext.view60.pose00.position00 = endPosition;
            RaceContext.view60.pose00.rotation10 = keys[endKey].view00.pose00.rotation10;
            RaceContext.view60.distance1C = keys[endKey].view00.distance1C;
            RaceContext.view60.projection20 = keys[endKey].view00.projection20;
        }
        ApplyCameraState(&RaceContext.view60);
        return 0;
    }

    orbitAngles.fields.x = 0;
    orbitAngles.fields.y = -0xA00;
    orbitAngles.fields.z = 0;
    orbitOffset.fields.x = D_800A0FC0.xyz.x;
    orbitOffset.fields.y = D_800A0FC0.xyz.y;
    orbitOffset.fields.z = D_800A0FC0.xyz.z;
    RotMatrix(&orbitAngles.fields, &rotation);
    ApplyMatrix(&rotation, &orbitOffset, &orbitPosition.xyz);
    orbitPosition.xyz.x += PlayerCars[0].position20.vector.x * 4U;
    orbitPosition.xyz.y += PlayerCars[0].position20.vector.y * 4U;
    orbitPosition.xyz.z += PlayerCars[0].position20.vector.z * 4U;

    if (duration > 0) {
        RaceContext.view60.pose00.position00.xyz.x = startPosition.xyz.x;
        RaceContext.view60.pose00.position00.xyz.y = startPosition.xyz.y;
        RaceContext.view60.pose00.position00.xyz.z = startPosition.xyz.z;
        RaceContext.view60.pose00.rotation10.x = keys[startKey].view00.pose00.rotation10.x;
        RaceContext.view60.pose00.rotation10.y = keys[startKey].view00.pose00.rotation10.y;
        RaceContext.view60.pose00.rotation10.z = keys[startKey].view00.pose00.rotation10.z;
        RaceContext.view60.distance1C = keys[startKey].view00.distance1C;
        RaceContext.view60.projection20 = keys[startKey].view00.projection20;
        ApplyCameraState(&RaceContext.view60);
        RaceContext.view60.pose00.position00 = projection->pose08.position00;
        RaceContext.view60.pose00.rotation10.x = projection->pose08.angles10.xyz.x;
        RaceContext.view60.pose00.rotation10.y = projection->pose08.angles10.xyz.y;
        RaceContext.view60.pose00.rotation10.z = projection->pose08.angles10.xyz.z;
        projection->pose08.position00.xyz.x = InterpolateCameraDelta(orbitPosition.xyz.x, RaceContext.view60.pose00.position00.xyz.x, elapsed, duration) + RaceContext.view60.pose00.position00.xyz.x;
        projection->pose08.position00.xyz.y = InterpolateCameraDelta(orbitPosition.xyz.y, RaceContext.view60.pose00.position00.xyz.y, elapsed, duration) + RaceContext.view60.pose00.position00.xyz.y;
        projection->pose08.position00.xyz.z = InterpolateCameraDelta(orbitPosition.xyz.z, RaceContext.view60.pose00.position00.xyz.z, elapsed, duration) + RaceContext.view60.pose00.position00.xyz.z;
        projection->pose08.angles10.xyz.x = InterpolateCameraDelta(D_800A0FD0.fields.x, RaceContext.view60.pose00.rotation10.x, elapsed, duration) + RaceContext.view60.pose00.rotation10.x;
        projection->pose08.angles10.xyz.y = InterpolateCameraDelta(D_800A0FD0.fields.y, RaceContext.view60.pose00.rotation10.y, elapsed, duration) + RaceContext.view60.pose00.rotation10.y;
        projection->pose08.angles10.xyz.z = InterpolateCameraDelta(D_800A0FD0.fields.z, RaceContext.view60.pose00.rotation10.z, elapsed, duration) + RaceContext.view60.pose00.rotation10.z;
        RaceContext.view60.projection20 = InterpolateCameraDelta(D_800A0FD8, keys[startKey].view00.projection20, elapsed, duration) + keys[startKey].view00.projection20;
        SetGeomScreen(RaceContext.view60.projection20);
    } else {
        projection->pose08.position00.xyz.x = orbitPosition.xyz.x;
        projection->pose08.position00.xyz.y = orbitPosition.xyz.y;
        projection->pose08.position00.xyz.z = orbitPosition.xyz.z;
        projection->pose08.angles10.xyz.x = D_800A0FD0.fields.x;
        projection->pose08.angles10.xyz.y = D_800A0FD0.fields.y;
        projection->pose08.angles10.xyz.z = D_800A0FD0.fields.z;
        SetGeomScreen(D_800A0FD8);
    }
    BuildCameraMatrices();
    return 0;
}

void DrawHudSprite(int a0, int a1, int a2, int a3, int a4)
{
    unsigned short sp[2];
    unsigned char *t0;
    SpritePacketSignedClut *p;
    t0 = D_800A0DC8[a0];
    if (a4 != 0) {
        sp[0] = 0xA0 - (t0[2] >> 1);
        sp[1] = a2;
    } else {
        sp[0] = a1;
        sp[1] = a2;
    }
    p = (SpritePacketSignedClut *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
    p->len = 4;
    p->code = 0x66;
    p->r = a3;
    p->g = a3;
    p->b = a3;
    p->x = sp[0];
    p->y = sp[1];
    p->w = t0[2];
    p->h = t0[3];
    p->u = t0[0];
    p->v = t0[1];
    p->clut = 0x7F38;
    p->addr = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
    CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = (unsigned long)p;
    p++;
    ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)p;
    p->len = 1;
    *((u32 *)p + 1) = 0xE1000229;
    p->addr = CurrentDrawBuf->buffer.ordering70.tags[0].bits.address;
    CurrentDrawBuf->buffer.ordering70.tags[0].bits.address = (unsigned long)p;
    ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)p + 8;
}

void DrawHudDigit(s32 a0, s32 a1, s32 a2, s32 a3) {
    SpritePacketSignedClut *p;
    s32 k;
    p = *(SpritePacketSignedClut **)SCRATCHPAD;
    k = 0x66;
    p->len = 4;
    p->code = k;
    p->r = a3; p->g = a3; p->b = a3;
    p->y = a2;
    p->u = a0 * 8;
    p->v = 0;
    p->w = 8;
    p->h = 0x10;
    p->x = a1;
    p->clut = 0x7F38;
    p->addr = ((SpritePacketSignedClut *)(&CurrentDrawBuf->buffer.ordering70.words[0]))->addr;
    *(u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]) = (*(u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]) & 0xFF000000) | ((u32)p & 0xFFFFFF);
    p++;
    *(SpritePacketSignedClut **)SCRATCHPAD = p;
    p->len = 1;
    *((u32 *)p + 1) = (SparkSpriteTexpage & 0x9FF) | 0xE1000200;
    p->addr = ((SpritePacketSignedClut *)(&CurrentDrawBuf->buffer.ordering70.words[0]))->addr;
    *(u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]) = (*(u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]) & 0xFF000000) | ((u32)p & 0xFFFFFF);
    *(SpritePacketSignedClut **)SCRATCHPAD = (SpritePacketSignedClut *)((u32 *)p + 2);
}

void UpdateSparkBurstEffectA(int a0, int a1) {
    unsigned short sp[2];
    int s0, s1;
    int s2;
    int s3;
    int d1, d2, d3, d4;
    s2 = a0;
    if (s2 < 0x10) {
        s0 = 0x10 - s2;
        sp[0] = 0x80 - s0;
    } else {
        s0 = 0;
        sp[0] = 0x80;
    }
    sp[1] = 0xD8;
    if (a1 < s2) {
        s1 = 0x80 - ((s2 - a1) * 4);
    } else {
        s1 = 0x80;
    }
    s3 = 0;
    d1 = s0 + 0x10;
    d2 = s0 - 0x36;
    d3 = s0 - 0x10;
    d4 = s0 + 0x36;
    do {
        DrawHudSprite(0, (short)(sp[0] - d1), (short)(sp[1] + d2), s1, 0);
        DrawHudSprite(1, (short)(sp[0] + d3), (short)(sp[1] - d4), s1, 0);
        s3++;
        sp[0] += 0x20;
        sp[0] += s0;
    } while (s3 < 3);
    if (s2 >= 0x10) {
        s2 -= 0x10;
        if (s2 < 0x10) s1 = s2 * 8;
        DrawHudSprite(2, 0, 0x110, s1, 1);
    }
}

void UpdateSparkBurstEffectB(int a0, int a1) {
  s16 v[4];
  u8 w[3];
  short *s1;
  int i;
  int c;
  unsigned short ua;
  s1 = D_800A0F20[RaceContext.camera24C->mode13C];
  do { c = a0 * 4U;
  if (a0 >= 0x20) {
    if (a1 < a0) c = 0x80 - ((a0 - a1) * 4);
    else c = 0x80;
  } } while (0);
  i = 0; v[0] = 0x7C; v[2] = 0xC4; v[3] = 0xB0; v[1] = 0xB0; w[2] = c; w[1] = c; w[0] = c;
  do { AddLineF2Prim(v, w, 1, 0); i += 1; v[1] += 1; v[3] += 1; } while (i < 2);
  i = 0; v[3] = 0x118; v[1] = 0x118; w[2] = c; w[1] = c; w[0] = c;
  do { AddLineF2Prim(v, w, 1, 0); i += 1; v[1] += 1; v[3] += 1; } while (i < 2);
  ua = s1[2];
  if (s1[2] >= 0) {
    do {
      DrawHudSprite((short) ua, s1[0], s1[1], c, s1[3]);
      s1 += 4;
      ua = s1[2];
    } while (s1[2] >= 0);
  }
  s1 = D_800A0F8C[SelectedTeamIndex];
  ua = s1[2];
  if (s1[2] >= 0) {
    do {
      DrawHudSprite((short) ua, s1[0], s1[1], c, s1[3]);
      s1 += 4;
      ua = s1[2];
    } while (s1[2] >= 0);
  }
}

void DrawSparkHudOverlay(int a0, int a1, int a2) {
    int s4 = 0x80;
    int s5 = 0;
    int s0 = 0;
    int s1 = 0;
    int r, q;
    if (a1 < 0x20) {
        s4 = a1 << 2;
    } else if (a2 < a1) {
        s4 -= (a1 - a2) << 2;
    }
    if (a1 < 0x30) {
        r = PsyqRand();
        q = r / 16;
        q = r - q * 16;
        s0 = q - 8;
        r = PsyqRand();
        q = r / 8;
        q = r - q * 8;
        s1 = q - 4;
        s5 = s4 / 4;
    }
    switch (a0) {
    case 0:
        DrawHudSprite(0x1A, 0, (short)(s0 + 0xD8), s4, 1);
        DrawHudSprite(0x1A, 0, (short)(s1 + 0xD8), s5, 1);
        break;
    case 1:
        DrawHudSprite(0xE, 0, (short)(s0 + 0xD8), s4, 1);
        DrawHudSprite(0xE, 0, (short)(s1 + 0xD8), s5, 1);
        s0 = (short)(s0 + 0xD8);
        s1 = (short)(s1 + 0xD8);
        DrawHudDigit(RaceContext.camera24C->mode140 / 10, 0xA4, s0, s4);
        DrawHudDigit(RaceContext.camera24C->mode140 / 10, 0xA4, s1, s5);
        DrawHudDigit(RaceContext.camera24C->mode140 % 10, 0xA8, s0, s4);
        DrawHudDigit(RaceContext.camera24C->mode140 % 10, 0xA8, s1, s5);
        break;
    case 2:
        DrawHudSprite(0x1B, 0, (short)(s0 + 0xD8), s4, 1);
        DrawHudSprite(0x1B, 0, (short)(s1 + 0xD8), s5, 1);
        break;
    }
}
