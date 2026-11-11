#include "types.h"
#include "scratchpad.h"
#include "game/car/car.h"
#include "game/car/carsp.h"
#include "game/car/aipath2.h"
#include "rr_asm.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/overlay668_camera.h"

void FindTrackNodeScratch(void)
{
    CarTrackWork *g = (CarTrackWork *)SCRATCH_CAR_WORK;
    s32 a, n, t, x;
    TrackSample3C *p, *q;
    s32 junk[2];
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r1 __asm__("$10");
    register s32 r2 __asm__("$11");
    register s32 r3 __asm__("$12");
    g->w38 = 0;
    for (;;) {
        g->w34 = (g->wheelState150.trackNodes[g->wheelIndex08] + 1) % TrackNodeCount;
        a = g->w34;
        p = &TrackNodes[g->wheelState150.trackNodes[g->wheelIndex08]];
        g->originX78 = p->road.x;
        g->originZ80 = p->road.z;
        q = &TrackNodes[a];
        g->nextLeft74.halves.x = g->nextRight70.halves.x = (u16)q->road.x - (u16)g->originX78;
        g->nextLeft74.halves.z = g->nextRight70.halves.z = (u16)q->road.z - (u16)g->originZ80;
        g->queryPoint64.halves.x = g->positions110[g->wheelIndex08].x - (u16)g->originX78;
        g->queryPoint64.halves.z = g->positions110[g->wheelIndex08].z - (u16)g->originZ80;
        g->nextRight70.halves.x += (u16)q->rightOffsetX00;
        g->nextRight70.halves.z += (u16)q->rightOffsetZ02;
        g->nextLeft74.halves.x += (u16)q->leftOffsetX04;
        g->nextLeft74.halves.z += (u16)q->leftOffsetZ06;
        g->rightEdge68.halves.x = (u16)p->rightOffsetX00;
        g->rightEdge68.halves.z = (u16)p->rightOffsetZ02;
        g->leftEdge6C.halves.x = (u16)p->leftOffsetX04;
        g->leftEdge6C.halves.z = (u16)p->leftOffsetZ06;
        r3 = g->queryPoint64.word; r1 = g->rightEdge68.word; r2 = g->leftEdge6C.word;
        RR_GTE_NCLIP3(r1, r2, r3, &g->w3C);
        r1 = g->nextLeft74.word; r2 = g->nextRight70.word; r3 = g->queryPoint64.word;
        RR_GTE_NCLIP3(r1, r2, r3, &g->w40);
        if (g->w3C >= 0 && g->w40 >= 0)
            return;
        n = g->w38 + 1;
        g->w38 = n;
        x = g->wheelState150.trackNodes[g->wheelIndex08];
        g->wheelState150.trackNodes[g->wheelIndex08] = (n & 1) ? x + n : x - n;
        a = g->wheelState150.trackNodes[g->wheelIndex08];
        if (a >= 0)
            g->wheelState150.trackNodes[g->wheelIndex08] = a % TrackNodeCount;
        else
            g->wheelState150.trackNodes[g->wheelIndex08] = (TrackNodeCount + a) % TrackNodeCount;
    }
}

void ConstrainCarToTrack(RaceVehicle *arg0) {
    CarTrackWork *g = (CarTrackWork *)SCRATCH_CAR_WORK;
    s32 idx, next_idx;
    TrackSample3C *s1, *s2;
    s32 i;
    s16 c, s;

    g->vehicle00 = arg0;
    g->positions110[0] = arg0->wheelPositions88[0];
    g->positions110[1] = arg0->wheelPositions88[1];
    g->positions110[2] = arg0->wheelPositions88[2];
    g->positions110[3] = arg0->wheelPositions88[3];
    g->vehicle00->impactFlags1A0 = 0;
    g->w2C = 0;
    g->sC0 = 0;
    g->sBA = 0;
    g->sC8 = 0;
    g->sC6 = 0;
    g->sC4 = 0;
    idx = g->vehicle00->trackNodeF0;
    next_idx = (idx + 1) % TrackNodeCount;
    s1 = &TrackNodes[idx];
    g->s174 = s1->road.rightAngle;
    g->s176 = s1->road.leftAngle;
    s2 = &TrackNodes[next_idx];
    g->s24 = rsin(g->s174);
    g->s26 = rcos(g->s174);
    g->s28 = rsin(g->s176);
    g->s2A = rcos(g->s176);
    if (g->s174 != g->s176) {
        g->w38 = 1;
        g->s162 = s1->road.segmentLength;
        g->w3C = g->s162;
        g->s160 = s1->road.heading + 0x400;
    } else {
        g->w38 = 0;
        g->s162 = (s1->road.segmentLength * g->s26) >> 12;
        g->w3C = g->s162;
        g->w40 = (g->s24 * s1->road.segmentLength) >> 12;
        g->s160 = s1->road.angle + 0x400;
    }
    g->sF8 = g->vehicle00->position20.vector.x - s1->road.x;
    g->sFC = g->vehicle00->position20.vector.z - s1->road.z;
    g->sFA = 0;
    g->w20 = rcos(g->s160);
    g->w1C = rsin(g->s160);
    c = g->w20;
    s = g->w1C;
    g->sB8 = g->sC2 = c;
    g->sBE = -s;
    g->sBC = s;
    RR_GTE_SETROT(&g->sB8);
    RR_GTE_MVMVA_ROT_V0_NOCV(&g->sF8);
    g->s15C = (s2->road.leftWidth + s2->road.leftExtension) - (s1->road.leftWidth + s1->road.leftExtension);
    g->s15A = (s2->road.rightWidth + s2->road.rightExtension) - (s1->road.rightWidth + s1->road.rightExtension);
    RR_GTE_STORE_IR(&g->translation100.vector.x);
    g->w14 = g->translation100.vector.x;
    g->w18 = g->translation100.vector.y;
    if (g->w38 == 1) {
        g->w40 = (g->w18 * g->s24) / g->s26;
        g->w14 -= g->w40;
        g->w3C -= g->w40;
        g->w3C += (g->w18 * g->s28) / g->s2A;
        g->w14 = (g->w14 * g->s162) / g->w3C;
    } else {
        g->w18 -= (g->w40 * g->w14) / g->s162;
    }
    if (g->w14 < 0) {
        g->w14 = 0;
    } else if (g->w14 > g->s162) {
        g->w14 = g->s162;
    }
    g->w3C = 0;
    g->w40 = 0;
    g->s24 = 0;
    g->s26 = 0;
    g->w10 = (g->w14 << 10) / g->s162;
    for (i = 0; i < 4; i++) {
        g->translation100.vector.x = g->sF8 + (g->positions110[i].x >> 2);
        g->translation100.vector.z = g->sFC + (g->positions110[i].z >> 2);
        g->w30 = (-g->w1C * g->translation100.vector.x + g->w20 * g->translation100.vector.z) >> 12;
        if (g->w30 > g->w3C) {
            g->w3C = g->w30;
            g->s24 = i + 1;
        } else if (g->w30 < g->w40) {
            g->w40 = g->w30;
            g->s26 = i + 1;
        }
    }
    g->s15C = ((g->s15C * g->w10) >> 10) + (s1->road.leftWidth + s1->road.leftExtension);
    g->s15A = ((g->s15A * g->w10) >> 10) + (s1->road.rightWidth + s1->road.rightExtension);
    if (g->w40 < -g->s15C) {
        g->sFC = g->w40 + g->s15C;
        g->w18 -= g->sFC;
        g->w2C = g->s26;
    } else if (g->w3C > g->s15A) {
        g->sFC = g->w3C - g->s15A;
        g->w18 -= g->sFC;
        g->w2C = g->s24;
    }
    if (g->w2C > 0) {
        g->sF8 = 0;
        g->sFA = 0;
        g->sB8 = g->w20;
        g->sBA = 0;
        g->sBC = -(s16)g->w1C;
        g->sBE = 0;
        g->sC0 = 0x1000;
        g->sC2 = 0;
        g->sC4 = g->w1C;
        g->sC6 = 0;
        g->sC8 = g->w20;
        RR_GTE_SETROT(&g->sB8);
        RR_GTE_MVMVA_ROT_V0_NOCV(&g->sF8);
        RR_GTE_STORE_IR(&g->translation100.vector.x);
        g->vehicle00->posScaled00[0] -= g->translation100.vector.x << 8;
        g->vehicle00->posScaled00[2] -= g->translation100.vector.z << 8;
        g->vehicle00->worldPosition10.words[0] = g->vehicle00->posScaled00[0] >> 6;
        g->vehicle00->worldPosition10.words[2] = g->vehicle00->posScaled00[2] >> 6;
    }
    g->s160 = s2->road.heading - s1->road.heading;
    if (g->s160 > 0x400) {
        g->s160 -= 0x1000;
    } else if (g->s160 < -0x400) {
        g->s160 = -0x1000 - g->s160;
    }
    g->vehicle00->trackAngle1EC = s1->road.heading + ((g->s160 * g->w14) >> 10);
    g->vehicle00->trackFraction170 = (g->w14 << 10) / g->s162;
    if (g->w18 < 0) {
        g->vehicle00->lateralOffset172 = (g->w18 << 10) / g->s15C;
    } else {
        g->vehicle00->lateralOffset172 = (g->w18 << 10) / g->s15A;
    }
    g->vehicle00->lateralVelF4 = g->w18;
    g->vehicle00->second = (ReverseCourse != 0) ? g->w14 : (g->s162 - g->w14);
    g->vehicle00->word190 = g->vehicle00->word18C;
    g->vehicle00->word18C = (g->vehicle00->first + g->vehicle00->second + TrackLength) % TrackLength;
    if (g->vehicle00->word18C < 0) {
        g->vehicle00->word18C += TrackLength;
    }
    g->vehicle00->courseDistance194 = (s1->road.distance + g->w14) >> 4;
    g->vehicle00->impactFlags1A0 = g->w2C;
}

