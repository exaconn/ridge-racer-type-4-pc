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
    SP *g = (SP *)SCRATCH_CAR_WORK;
    s32 a, n, t, x;
    TrackSample3C *p, *q;
    s32 junk[2];
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r1 __asm__("$10");
    register s32 r2 __asm__("$11");
    register s32 r3 __asm__("$12");
    g->cnt = 0;
    for (;;) {
        g->w34 = (g->s150[g->idx] + 1) % TrackNodeCount;
        a = g->w34;
        p = &TrackNodes[g->s150[g->idx]];
        g->w78 = p->x08;
        g->w80 = p->z0C;
        q = &TrackNodes[a];
        g->h74 = g->h70 = (u16)q->x08 - (u16)g->w78;
        g->h76 = g->h72 = (u16)q->z0C - (u16)g->w80;
        g->h64 = g->t110[g->idx].a - (u16)g->w78;
        g->h66 = g->t110[g->idx].c - (u16)g->w80;
        g->h70 += (u16)q->rightOffsetX00;
        g->h72 += (u16)q->rightOffsetZ02;
        g->h74 += (u16)q->leftOffsetX04;
        g->h76 += (u16)q->leftOffsetZ06;
        g->h68 = (u16)p->rightOffsetX00;
        g->h6a = (u16)p->rightOffsetZ02;
        g->h6c = (u16)p->leftOffsetX04;
        g->h6e = (u16)p->leftOffsetZ06;
        r3 = *(s32 *)&g->h64; r1 = *(s32 *)&g->h68; r2 = *(s32 *)&g->h6c;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r3c);
        r1 = *(s32 *)&g->h74; r2 = *(s32 *)&g->h70; r3 = *(s32 *)&g->h64;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r40);
        if (g->r3c >= 0 && g->r40 >= 0)
            return;
        n = g->cnt + 1;
        g->cnt = n;
        x = g->s150[g->idx];
        g->s150[g->idx] = (n & 1) ? x + n : x - n;
        a = g->s150[g->idx];
        if (a >= 0)
            g->s150[g->idx] = a % TrackNodeCount;
        else
            g->s150[g->idx] = (TrackNodeCount + a) % TrackNodeCount;
    }
}

void ConstrainCarToTrack(RaceVehicle *arg0) {
    SP_1 *g = (SP_1 *)SCRATCH_CAR_WORK;
    s32 idx, next_idx;
    TrackSample3C *s1, *s2;
    s32 i;
    s16 c, s;

    g->p0 = arg0;
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[0] = arg0->wheelPositions88[0];
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[1] = arg0->wheelPositions88[1];
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[2] = arg0->wheelPositions88[2];
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[3] = arg0->wheelPositions88[3];
    g->p0->impactFlags1A0 = 0;
    g->w2C = 0;
    g->sC0 = 0;
    g->sBA = 0;
    g->sC8 = 0;
    g->sC6 = 0;
    g->sC4 = 0;
    idx = g->p0->trackNodeF0;
    next_idx = (idx + 1) % TrackNodeCount;
    s1 = &TrackNodes[idx];
    g->s174 = s1->angleRight22;
    g->s176 = s1->angleLeft20;
    s2 = &TrackNodes[next_idx];
    g->s24 = rsin(g->s174);
    g->s26 = rcos(g->s174);
    g->s28 = rsin(g->s176);
    g->s2A = rcos(g->s176);
    if (g->s174 != g->s176) {
        g->w38 = 1;
        g->s162 = s1->segmentLength1C;
        g->w3C = g->s162;
        g->s160 = s1->heading12 + 0x400;
    } else {
        g->w38 = 0;
        g->s162 = (s1->segmentLength1C * g->s26) >> 12;
        g->w3C = g->s162;
        g->w40 = (g->s24 * s1->segmentLength1C) >> 12;
        g->s160 = s1->angle1E + 0x400;
    }
    g->sF8 = g->p0->position20.vector.x - s1->x08;
    g->sFC = g->p0->position20.vector.z - s1->z0C;
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
    g->s15C = (s2->leftWidth18 + s2->leftExtension2E) - (s1->leftWidth18 + s1->leftExtension2E);
    g->s15A = (s2->rightWidth1A + s2->rightExtension2C) - (s1->rightWidth1A + s1->rightExtension2C);
    RR_GTE_STORE_IR(&g->w100);
    g->w14 = g->w100;
    g->w18 = g->w104;
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
        g->w100 = g->sF8 + (g->t110[i].a >> 2);
        g->w108 = g->sFC + (g->t110[i].c >> 2);
        g->w30 = (-g->w1C * g->w100 + g->w20 * g->w108) >> 12;
        if (g->w30 > g->w3C) {
            g->w3C = g->w30;
            g->s24 = i + 1;
        } else if (g->w30 < g->w40) {
            g->w40 = g->w30;
            g->s26 = i + 1;
        }
    }
    g->s15C = ((g->s15C * g->w10) >> 10) + (s1->leftWidth18 + s1->leftExtension2E);
    g->s15A = ((g->s15A * g->w10) >> 10) + (s1->rightWidth1A + s1->rightExtension2C);
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
        RR_GTE_STORE_IR(&g->w100);
        g->p0->posScaled00[0] -= g->w100 << 8;
        g->p0->posScaled00[2] -= g->w108 << 8;
        g->p0->pos10[0] = g->p0->posScaled00[0] >> 6;
        g->p0->pos10[2] = g->p0->posScaled00[2] >> 6;
    }
    g->s160 = s2->heading12 - s1->heading12;
    if (g->s160 > 0x400) {
        g->s160 -= 0x1000;
    } else if (g->s160 < -0x400) {
        g->s160 = -0x1000 - g->s160;
    }
    g->p0->trackAngle1EC = s1->heading12 + ((g->s160 * g->w14) >> 10);
    g->p0->trackFraction170 = (g->w14 << 10) / g->s162;
    if (g->w18 < 0) {
        g->p0->lateralOffset172 = (g->w18 << 10) / g->s15C;
    } else {
        g->p0->lateralOffset172 = (g->w18 << 10) / g->s15A;
    }
    g->p0->lateralVelF4 = g->w18;
    g->p0->second = (ReverseCourse != 0) ? g->w14 : (g->s162 - g->w14);
    g->p0->word190 = g->p0->word18C;
    g->p0->word18C = (g->p0->first + g->p0->second + TrackLength) % TrackLength;
    if (g->p0->word18C < 0) {
        g->p0->word18C += TrackLength;
    }
    g->p0->courseDistance194 = (s1->dist24 + g->w14) >> 4;
    g->p0->impactFlags1A0 = g->w2C;
}

