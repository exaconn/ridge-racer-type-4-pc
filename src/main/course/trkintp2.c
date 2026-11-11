#include "rr/scene_camera_state.h"
#include "game/gfx/dispenv.h"
#include "psyq/libgte/gtemat5.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat3.h"
#include "scratchpad.h"
#include "rr/track_interpolation.h"
#include "game/ui/uicode.h"
#include "game/ui/uicnt.h"
#include "game/course/trkintp2.h"
#include "rr/overlay662_palette_refresh.h"
#include "rr/matrix_workspace.h"
#include "rr/overlay665_render.h"
#include "game/math/rotmat.h"

typedef struct { int x, y, z, w; } V4;

typedef struct {
    int w0[2];
    V4 pos08;
    V4 vec18;
} ScratchTrackView;

#include "game/ui/uicnt.h"
extern int PsyqStrcpy();
extern int PsyqRand();
extern char PasswordChars[];
extern char PasswordBytes[];
extern void DrawSync();

int ValidateRecordPassword(unsigned char *a0, unsigned char *a1);
int EncodeRecordPassword();
int DecodeRecordPassword(int *a, int *b, int *c, int *d, int *e, int f);
void LoadTimImage(int *p);
void LoadTimList(int *p);
void GetTrackPointAhead(RaceVehicle *vehicle, s32 *output, s32 distance);

int ValidateRecordPassword(unsigned char *a0, unsigned char *a1) {
    unsigned char *p = a1;
    unsigned char *q;
    unsigned char *r;
    int i;
    int k;
    int c;
    unsigned v;
    unsigned char *tab;
    c = CodeCharsToByte(p[0], p[1]);
    i = 0;
    q = a0;
L1:
    {
        unsigned char t0 = p[2];
        unsigned char t1 = p[3];
        p += 2;
        i++;
        *q = CodeCharsToByte(t0, t1);
        q++;
    }
    if (i < 6) goto L1;
    q = a0;
    i = 0;
    k = ((unsigned char)c % 11) & 0xFF;
    tab = PasswordCipherKey;
    do {
        unsigned char *t;
        r = (unsigned char *)(k + (int)tab);
        t = q + i;
        k++;
        *t = AddOrSubtractByte(*t, *r, 1);
        if (k >= 11) k = 0;
        i++;
    } while (i < 6);
    a0[5] = a0[5] & 0xF;
    if ((unsigned char)c != (u8)ChecksumBytes(a0, 6)) return -1;
    XorBytes37(a0, 6);
    v = (((a0[4] & 0xF) << 16) + (a0[3] << 8) + a0[2]);
    v >>= 2;
    if (v > 0x927BF) return -1;
    v = (a0[1] << 4) + (a0[0] >> 4);
    v &= 0x3FF;
    if (v >= 0x280) return -1;
    v = a0[5] & 0xF;
    if (v >= 6) return -1;
    return 0;
}

int EncodeRecordPassword(int a0, int a1, int a2, int a3, int a4, int a5) {
    unsigned s0; int r;
    s0 = a1 << 4;
    a0 &= 0xF;
    s0 |= a0;
    PasswordBytes[0] = s0;
    r = PsyqRand(a0);
    a2 = a2 * 4 + a3;
    PasswordBytes[1] = (s0 >> 8) + (r & ~0x3F);
    PasswordBytes[2] = a2;
    PasswordBytes[3] = a2 >> 8;
    PasswordBytes[4] = (a2 >> 16) + (PsyqRand() & ~0x3F);
    PasswordBytes[5] = a4 & 0xF;
    EncodePasswordCode(PasswordBytes, PasswordChars);
    PasswordChars[0xD] = 0;
    PsyqStrcpy(a5, PasswordChars);
    return 0;
}

int DecodeRecordPassword(int *a, int *b, int *c, int *d, int *e, int f) {
    unsigned t;
    PsyqStrcpy(PasswordChars, f);
    if (ValidateRecordPassword(PasswordBytes, PasswordChars) == 0) {
        *a = PasswordBytes[0] & 0xF;
        *b = (PasswordBytes[1] * 16 + (PasswordBytes[0] >> 4)) & 0x3FF;
        t = (PasswordBytes[4] << 16) + (PasswordBytes[3] << 8) + PasswordBytes[2];
        *c = (t >> 2) & 0xFFFFF;
        *d = t & 3;
        *e = PasswordBytes[5] & 0xF;
        return 0;
    }
    return -1;
}

void LoadTimImage(int *p) {
    short r[4];
    unsigned short *h;
    p++;
    if (*p++ & 8) {
        h = (unsigned short *)p;
        r[0] = h[2];
        r[1] = h[3];
        r[2] = h[4];
        r[3] = h[5];
        LoadImage(r, h + 6);
        DrawSync(0);
        p = (int *)((char *)h + (*(unsigned *)h >> 2 << 2));
    }
    h = (unsigned short *)p;
    r[0] = h[2];
    r[1] = h[3];
    r[2] = h[4];
    r[3] = h[5];
    if (r[2] > 0 && r[3] > 0) {
        LoadImage(r, h + 6);
        DrawSync(0);
    }
}

void LoadTimList(int *p) {
    int n;
    p++;
    while ((n = *p++) > 0) {
        char *q = (char *)p + ((unsigned)n >> 2 << 2);
        LoadTimImage(p);
        p = (int *)q;
    }
}

int FindNearestTrackEntry(RaceVehicle *vehicle) {
    int t2 = 0;
    int t1 = 0x7FFFFFFF;
    CamNode44 *a2 = RaceContext.cameraNodes550;
    unsigned short a1 = a2->node32;
    int a3 = 0;
    int a0v, v1, b;
    if (a2->node32 != -1) {
        a0v = vehicle->trackNodeF0;
        do {
            v1 = (short)a1;
            if (v1 < a0v) v1 = a0v - v1; else v1 = v1 - a0v;
            b = v1;
            if (TrackNodeCount / 2 < v1) b = TrackNodeCount - v1;
            v1 = b;
            if (v1 < t1) { t2 = a3; t1 = v1; }
            a2++;
            a1 = a2->node32;
            a3++;
        } while (a2->node32 != -1);
    }
    return t2;
}

void InitDemoComCars(char *a0, int *a1) {
    struct {
        /* Preserve the existing work capacity; its extra half is unresolved. */
        union { MatrixWords32 matrix; s32 storage[16]; } m;
        MatrixWords32 n;
        short c[4], d[4];
        V4 e, f;
    } s;
    int *g;
    RaceVehicle *car = (RaceVehicle *)a0;
    BuildRotMatrixY(&s.m.matrix, 0x800 - car->ang82);
    BuildRotMatrixX(&s.n, car->ang80);
    MulMatrix(&s.m.matrix, &s.n);
    BuildRotMatrixZ(&s.n, car->ang84);
    MulMatrix2(&s.m.matrix, &s.n);
    s.c[0] = a1[0] * 4;
    s.c[1] = a1[1] * 4;
    s.c[2] = a1[2] * 4;
    ApplyMatrix(&s.n, (const Vec3s8 *)(s.c), (Vec3i *)(&s.e));
    BuildRotMatrixY(&s.m.matrix, a1[5]);
    BuildRotMatrixX(&s.n, car->ang80);
    MulMatrix(&s.m.matrix, &s.n);
    BuildRotMatrixZ(&s.n, car->ang84);
    MulMatrix2(&s.m.matrix, &s.n);
    s.d[0] = (unsigned short)car->ang80;
    s.d[1] = 0;
    s.d[2] = (unsigned short)car->ang84;
    ApplyMatrix(&s.n, (const Vec3s8 *)(s.d), (Vec3i *)(&s.f));
    ((ScratchTrackView *)SCRATCHPAD)->vec18 = s.f;
    g = (int *)SCRATCHPAD;
    g[7] = car->ang82 + a1[5];
    g[6] += a1[4];
    g[8] += a1[6];
    ((ScratchTrackView *)SCRATCHPAD)->pos08 = *(V4 *)car->ir70;
    g[2] += s.e.x;
    g[3] += s.e.y;
    g[4] += s.e.z;
    SetGeomScreen(a1[10]);
}

void CarLocalToWorld(RaceVehicle *vehicle, const Vec3s8 *offset, Vector *output) {
    MatrixWords32 yaw;
    /* The existing two spare words remain reconstruction debt. */
    union { MatrixWords32 matrix; s32 storage[10]; } orientation;
    Vec3i transformed;
    BuildRotMatrixY(&yaw, 0x800 - vehicle->ang82);
    BuildRotMatrixX(&orientation.matrix, vehicle->ang80);
    MulMatrix(&yaw, &orientation.matrix);
    BuildRotMatrixZ(&orientation.matrix, vehicle->ang84);
    MulMatrix2(&yaw, &orientation.matrix);
    ApplyMatrix(&orientation.matrix, offset, &transformed);
    *output = vehicle->worldPosition10.vector;
    output->x = (s32)((u32)output->x + transformed.x);
    output->y = (s32)((u32)output->y + transformed.y);
    output->z = (s32)((u32)output->z + transformed.z);
}

void GetTrackPointAhead(RaceVehicle *vehicle, s32 *output, s32 distance) {
    s16 current_y;
    s32 fraction;
    s32 index, length;
    u8 *page;
    s32 neighbour;
    TrackSample3C *next;
    TrackSample3C *current;

    if (ReverseCourse == 0) {
        index = vehicle->trackNodeF0 - ((distance - vehicle->trackFraction170) >> 0xA);
        neighbour = index - 1;
        if (index < 0) {
            index += TrackNodeCount;
            neighbour = index - 1;
        }
        if (neighbour < 0) {
            neighbour += TrackNodeCount;
        }
    } else {
        page=(u8*)0x800B0000U;
        length=((s32*)page)[-0x4134/4];
        index = vehicle->trackNodeF0 + ((distance - vehicle->trackFraction170) >> 0xA);
        neighbour = index + 1;
        if (index >= length) {
            index -= length;
            neighbour = index + 1;
        }
        /* This signed test is present in the original, including negative indices. */
        if (neighbour >= 0) {
            neighbour -= length;
        }
    }
    current = TrackNodes + index;
    next = TrackNodes + neighbour;
    fraction = (distance - vehicle->trackFraction170) & 0x3FF;
    output[0] = (((((next->road.x - current->road.x) * fraction) / 1024) + current->road.x) * 4);
    current_y = current->road.height;
    output[1] = ((((next->road.height - current_y) * fraction) / 1024) + current_y);
    output[2] = (((((next->road.z - current->road.z) * fraction) / 1024) + current->road.z) * 4);
}
