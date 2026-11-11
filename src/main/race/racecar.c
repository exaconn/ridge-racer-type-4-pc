#include "scratchpad.h"
#include "game/car/carmove1.h"
#include "game/car/carparam.h"
#include "game/car/carsetup.h"
#include "rr/overlay662_distance.h"
#include "game/car/aipath.h"
#include "game/car/carphys2.h"
#include "game/car/aipath2.h"
#include "game/car/carsteer.h"
#include "game/race/racecar.h"
#include "game/course/trkintp.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/overlay668_camera.h"

extern int ViewUsesCarTrackNode;

typedef struct { int a, b, c, d; } w3g2V;
typedef struct { int unk00[2]; w3g2V pos08; } ScratchCarView;

void UpdateFinishingPlayerCar(RaceVehicle *p) {
    int sp[3];
    int v, t, u, v2, v3, v4;
    int *g = (int *)SCRATCHPAD;
    int *q = (int *)(SCRATCHPAD + 0x7C);
    ViewUsesCarTrackNode = 0;
    u = p->input;
    p->input = (unsigned short)p->input - (u * 6) / 100;
    p->input = p->input + *(u16 *)&p->physicsFlags1E0;
    v = p->input * rsin(p->heading1D4 + (p->half1EA << 11));
    if (v < 0) v += 0xFF;
    p->vel30 = v >> 8;
    v2 = p->input * rcos(p->heading1D4 + (p->half1EA << 11));
    if (v2 < 0) v2 += 0xFF;
    p->vel38 = v2 >> 8;
    SteerCarTowardTrack(p);
    t = AngleDiffSigned(p->rot54, p->heading1D4);
    *q = 0;
    p->rot54 = p->rot54 + t / 2;
    ClampAiCarLateralMotion(p);
    UpdateCarSteering(p);
    IntegrateCarMotion(p);
    UpdateCarTrackNode(p);
    ComputeWheelOffsets(p);
    ConstrainCarToTrack(p);
    FindWheelTrackNodes(p);
    UpdateWheelBumps(p);
    v3 = rsin(PlayerCars[0].rot54);
    if (v3 < 0) v3 += 0x7F;
    sp[0] = PlayerCars[0].worldPosition10.words[0] + (v3 >> 7) - g[2];
    sp[1] = PlayerCars[0].worldPosition10.words[1] - g[3];
    v4 = rcos(PlayerCars[0].rot54);
    if (v4 < 0) v4 += 0x7F;
    sp[2] = PlayerCars[0].worldPosition10.words[2] + (v4 >> 7) - g[4];
    ((ScratchCarView *)SCRATCHPAD)->pos08 = *(w3g2V *)p->worldPosition10.words;
    g[3] -= 0x100;
    g[7] = 0x400 - Atan2Table(sp[0], sp[2]);
    g[6] = 0x400 - Atan2Table(sp[1], CordicSqrt(sp[0] * sp[0] + sp[2] * sp[2]) >> 6);
    g[8] = 0;
}
