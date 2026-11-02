#include "types.h"

typedef struct {
    s16 x;
    s16 y;
} V;

typedef struct {
    char pad0[0x12];
    s16 f12;
    char pad14[4];
    s16 f18;
    s16 f1A;
    s16 f1C;
    char pad1E[0xE];
    s16 f2C;
    s16 f2E;
    char pad30[0xC];
} TrackNode;

extern TrackNode *D_800ABEA4;
extern s32 D_800ABECC;
extern s16 D_800A68C0[];

#define SIN(x) (((x) & 0x800) ? (((x) & 0x400) ? -D_800A68C0[0x400 - ((x) & 0x3FF)] : -D_800A68C0[(x) & 0x3FF]) : (((x) & 0x400) ? D_800A68C0[0x400 - ((x) & 0x3FF)] : D_800A68C0[(x) & 0x3FF]))

void func_800464E8(V a0, V a1, s32 a2, s16 *a3, s16 *a4, s32 *a5) {
    s32 next_idx;
    TrackNode *t2;
    TrackNode *t3;
    s32 ang_sin;
    s32 c;
    s32 t1;
    s32 t0;
    s32 q;
    s32 w2;
    s32 w3;

    next_idx = (a2 + 1) % D_800ABECC;
    t2 = &D_800ABEA4[a2];
    t3 = &D_800ABEA4[next_idx];

    ang_sin = t2->f12 + 0x800;
    a2 = t2->f12 + 0x400;

    t0 = SIN(ang_sin);
    c = SIN(a2);

    t1 = (t0 * a0.x + c * a1.x) / 4096;
    t0 = (-c * a0.x + t0 * a1.x) / 4096;

    if (t1 < 0) {
        t1 = 0;
    }

    a2 = t3->f12 - t2->f12;
    a2 = t2->f1C - ((t0 * SIN(a2)) / 4096);
    if (t1 > a2) {
        t1 = a2;
    }

    q = (t1 << 10) / a2;
    *a3 = q;
    *a5 = a2 - ((a2 * (s16)q) >> 10);

    if (t0 < 0) {
        w2 = t2->f18 + t2->f2E;
        w3 = t3->f18 + t3->f2E;
        a2 = w2 - w3;
        if (a2 == 0) {
            *a4 = (t0 << 10) / w2;
        } else {
            a2 = ((a2 * *a3) / 1024) + w3;
            *a4 = (t0 << 10) / a2;
        }
    } else {
        w2 = t2->f1A + t2->f2C;
        w3 = t3->f1A + t3->f2C;
        a2 = w2 - w3;
        if (a2 == 0) {
            *a4 = (t0 << 10) / w2;
        } else {
            a2 = ((a2 * *a3) / 1024) + w3;
            *a4 = (t0 << 10) / a2;
        }
    }
}
