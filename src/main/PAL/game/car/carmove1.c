#include "types.h"
#include "scratchpad.h"
#include "game/car/car.h"
#include "game/car/carsp.h"
#include "rr_asm.h"
#include "game/car/carstate.h"
#include "game/car/carwheel.h"
#include "game/car/carmove1.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/overlay668_camera.h"

typedef struct Node {
    s32 off0;
    s32 off4;
    Z52 z8;
} Node;

typedef struct {
    s32 w0[2];
    int s8;
    s32 wC[10];
    int w34, w38, w3c, w40;
    s32 w44[52];
    int w114[3][4];
    int w144[3];
    short h150[4];
    s32 w158[62];
    short h250[2][4];
} T;

struct TrackHeader4 { char b[4]; };

extern short TrackLengthKilometers;
extern short D_800ACC74;

void FindAxleTrackNode(void)
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
        g->h64 = (g->t110[g->idx].a + g->t110[g->idx + 1].a) / 2 - (u16)g->w78;
        g->h66 = (g->t110[g->idx].c + g->t110[g->idx + 1].c) / 2 - (u16)g->w80;
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
        x = g->s150[g->idx] + TrackNodeCount;
        g->s150[g->idx] = ((n & 1) ? x + n : x - n) % TrackNodeCount;
    }
}

void FindWheelTrackNodes(RaceVehicle *arg0) {
    SP_1 *g = (SP_1 *)SCRATCH_CAR_WORK;
    s32 t0;
    s32 *q;

    arg0->contactFlags1F0.word = 0;
    arg0->position20.vector.x = arg0->pos10[0] >> 2;
    arg0->position20.vector.z = arg0->pos10[2] >> 2;
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[0] = arg0->wheelPositions88[0];
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[1] = arg0->wheelPositions88[1];
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[2] = arg0->wheelPositions88[2];
    ((ScratchCarPad *)SCRATCHPAD)->wheels190[3] = arg0->wheelPositions88[3];
    g->sC0 = 0;
    g->sBA = 0;
    g->sC8 = 0;
    g->sC6 = 0;
    g->sC4 = 0;
    g->sE6 = 0;
    g->sE4 = 0;
    g->sE2 = 0;
    g->sDE = 0;
    g->sDC = 0;
    g->sDA = 0;
    ((ScratchCarPad *)SCRATCHPAD)->pos180 = arg0->position20.vector;
    q = (s32 *)(SCRATCHPAD + 0x7C);
    {
        s32 a0 = arg0->trackNodeF0;
        s32 v0 = a0 - 2;
        s32 v1 = (v0 + TrackNodeCount) % TrackNodeCount;
        g->w14 = a0;
        g->w30 = arg0->width10C;
        g->w8 = 0;
        g->w20 = v1;
    }
    do {
        g->t110[g->w8].a = g->w100 + (g->t110[g->w8].a >> 2);
        g->t110[g->w8].c = g->w108 + (g->t110[g->w8].c >> 2);
        g->t110[g->w8 + 1].a = g->w100 + (g->t110[g->w8 + 1].a >> 2);
        g->t110[g->w8 + 1].c = g->w108 + (g->t110[g->w8 + 1].c >> 2);
        g->s150[g->w8] = arg0->wheelNodesF8[g->w8];
        FindAxleTrackNode();
        g->z180[g->w8] = ((Node *)TrackNodes)[g->s150[g->w8]].z8;
        g->z180[g->w8 + 1] = ((Node *)TrackNodes)[(g->s150[g->w8] + 1) % TrackNodeCount].z8;
        arg0->wheelNodesF8[g->w8 + 1] = arg0->wheelNodesF8[g->w8] = g->s150[g->w8];
        g->w8 += 2;
    } while (g->w8 < 3);
    g->w8 = 0;
    do {
        s32 a0 = g->w8;
        g->wC = a0;
        if (g->s150[0] != g->s150[2] || g->w8 < 2) {
            g->s174 = g->z180[a0].s1A;
            g->s176 = g->z180[g->wC].s18;
            g->s24 = rsin(g->s174);
            g->s26 = rcos(g->s174);
            g->s28 = rsin(g->s176);
            g->s2A = rcos(g->s176);
            if (g->s174 != g->s176) {
                g->w38 = 1;
                g->s174 = g->s162 = g->z180[g->wC].s14;
                g->s160 = g->z180[g->wC].sA + 0x400;
            } else {
                g->w38 = 0;
                g->s174 = g->s162 = (g->z180[g->wC].s14 * g->s26) >> 12;
                g->s160 = g->z180[g->wC].s16 + 0x400;
                g->s176 = (g->s24 * g->z180[g->wC].s14) >> 12;
            }
            g->sB8 = g->sC2 = rcos(g->s160);
            {
                s16 s = rsin(g->s160);
                g->sBE = -s;
                g->sBC = s;
            }
            RR_GTE_SETROT(&g->sB8);
            g->s16C = g->z180[g->wC + 1].s8 - g->z180[g->wC].s8;
            g->s168 = g->z180[g->wC + 1].sE - g->z180[g->wC].sE;
            g->s164 = g->z180[g->wC + 1].s12 - g->z180[g->wC].s12;
            g->s166 = g->z180[g->wC + 1].s10 - g->z180[g->wC].s10;
        }
        for (t0 = 0; t0 < 2; t0++) {
            g->sF8 = g->t110[g->w8 + t0].a - g->z180[g->wC].w0;
            g->sFC = g->t110[g->w8 + t0].c - g->z180[g->wC].w4;
            RR_GTE_MVMVA_ROT_V0_NOCV(&g->sF8);
            g->w34 = 0;
            g->s16E = 0;
            g->w3C = g->s174;
            g->w40 = g->s176;
            RR_GTE_STORE_IR(&g->w14);
            __asm__ ("" : : : "memory"); /* ASM_BARRIER: RR_GTE_STORE_IR writes g->w14..pad1C in memory */
            if (g->w38 == 1) {
                g->w40 = (g->w18 * g->s24) / g->s26;
                g->w14 -= g->w40;
                g->w3C -= g->w40;
                g->w3C += (g->w18 * g->s28) / g->s2A;
                g->w14 = (g->w14 * g->s162) / g->w3C;
            } else {
                g->w18 -= (g->w40 * g->w14) / g->s162;
            }
            g->w10 = (g->w14 << 10) / g->s162;
            g->s158 = ((g->s168 * g->w10) >> 10) + g->z180[g->wC].sE;
            g->s15A = ((g->s164 * g->w10) >> 10) + g->z180[g->wC].s12;
            g->s15C = ((g->s166 * g->w10) >> 10) + g->z180[g->wC].s10;
            if (g->w18 > g->s15A) {
                g->s16A = g->w18 - g->s15A;
                g->w34 = 1;
                g->w18 = g->s15A;
            } else if (g->w18 < -g->s15C) {
                g->s16A = -(g->w18 + g->s15C);
                g->w34 = 2;
                g->w18 = -g->s15C;
            }
            if (g->w34 > 0) {
                g->w34--;
                g->s164 = g->z180[g->wC + 1].s2C[g->w34] - g->z180[g->wC].s2C[g->w34];
                g->s164 = ((g->s164 * g->w10) >> 10) + g->z180[g->wC].s2C[g->w34];
                g->s170 = g->z180[g->wC + 1].s30[g->w34] - g->z180[g->wC].s30[g->w34];
                g->s170 = ((g->s170 * g->w10) >> 10) + g->z180[g->wC].s30[g->w34];
                if (g->s16A < g->s164) {
                    g->s172 = g->s16A;
                    g->s16A = 0;
                } else {
                    g->s16A -= (g->s172 = g->s164);
                    g->s16E = g->z180[g->wC + 1].s28[g->w34] - g->z180[g->wC].s28[g->w34];
                    g->s16E = ((g->s16E * g->w10) >> 10) + g->z180[g->wC].s28[g->w34];
                    {
                        s32 v = (g->w34 != 0) ? -(((g->s16E * g->s16A) << 11) >> 16) : (((g->s16E * g->s16A) << 11) >> 16);
                        g->s16E = v;
                    }
                    arg0->contactFlags1F0.bytes[g->w8 + t0] = g->z180[g->wC].a20[g->w34].b0;
                }
                {
                    s32 v = (g->w34 != 0) ? -(((g->s170 * g->s172) << 11) >> 16) : (((g->s170 * g->s172) << 11) >> 16);
                    g->s16E += (g->s170 = v);
                }
            }
            g->s250[*q][g->w8 + t0] = g->s16E + ((((g->s16C * g->w10) >> 10) + g->z180[g->wC].s8) + ((g->s158 * g->w18) >> 5));
        }
        g->w8 += 2;
    } while (g->w8 < 4);
}

void UpdateWheelBumps(RaceVehicle *a)
{
    int *q7c = (int *)(SCRATCHPAD + 0x7C);
    T *SC = (T *)SCRATCH_CAR_WORK;
    int t, t2, t3, v;
    RaceVehicle *p2;
    int *pw;
    SC->s8 = 0;
    do {
        t = SC->s8;
        SC->w114[t][0] = SC->h250[*q7c][t];
        if (a->prevWheelNodes100[SC->s8] != a->wheelNodesF8[SC->s8]) {
            if (a->wheelBumpState1BC[SC->s8] == 0) {
                if (TriggerWheelBump(a, SC->s8) != 0)
                    a->wheelBumpBase1CC[SC->s8] = SC->w114[SC->s8][0];
            }
        }
        t2 = *(int *)&SC->s8;
        p2 = (RaceVehicle *)((char *)a + t2 * 2);
        {
            int c = p2->wheelBumpState1BC[0];
            if (c > 0) {
                int lo = p2->wheelBumpVel1C4[0] * c;
                a->wheelPositions88[t2].y = p2->wheelBumpBase1CC[0] + lo + ((c * c << 11) / 900);
                t3 = SC->s8;
                v = SC->w114[t3][0];
                if (a->wheelPositions88[t3].y >= v && a->wheelBumpState1BC[t3] >= 2) {
                    a->wheelPositions88[t3].y = v;
                    if (SC->s8 == 0) {
                        a->steerInput1BA = a->wheelBumpState1BC[0];
                        SetCarImpactState(1, a);
                    }
                    a->wheelBumpState1BC[SC->s8] = 0;
                    a->steeringLock1B8 ^= 1 << SC->s8;
                } else {
                    a->wheelBumpState1BC[SC->s8]++;
                }
                SC->h150[SC->s8] = a->wheelPositions88[SC->s8].y - SC->w114[SC->s8][0];
            } else {
                a->wheelPositions88[t2].y = SC->w114[t2][0];
                SC->h150[SC->s8] = 0;
            }
        }
        a->prevWheelNodes100[SC->s8] = a->wheelNodesF8[SC->s8];
    } while (++SC->s8 < 4);
    a->wheelRotX1A8 += Atan2Table(a->halfSpan10E, (SC->h150[0] + SC->h150[1] - (SC->h150[2] + SC->h150[3])) >> 3);
    a->wheelRotZ1AA += Atan2Table(a->width10C, (SC->h150[0] + SC->h150[2] - (SC->h150[1] + SC->h150[3])) >> 3);
    {
        int m = (a->wheelPositions88[2].y + a->wheelPositions88[3].y) >> 1;
        a->pos10[1] = m;
        a->pos10[1] = ((((a->wheelPositions88[0].y + a->wheelPositions88[1].y) - (a->wheelPositions88[2].y + a->wheelPositions88[3].y)) >> 1) * 0x14) / a->halfSpan10E + m;
    }
    {
        int m = (SC->w114[2][0] + SC->w114[3][0]) >> 1;
        int v, d;
        a->bodyPosition.y = m;
        d = ((((SC->w114[0][0] + SC->w114[1][0]) - (SC->w114[2][0] + SC->w114[3][0])) >> 1) * 0x14) / a->halfSpan10E;
        v = a->sampleCount16C & 0xF;
        a->bodyPosition.y = d + m;
        a->sampleCount16C = v;
        SC->w34 = v;
        SC->w38 = (v + 6) & 0xF;
        SC->w3c = (SC->w34 + 0xC) & 0xF;
        SC->w40 = (SC->w34 + 2) & 0xF;
    }
    a->xHistory124[SC->w34] = Atan2Table(a->halfSpan10E, (a->wheelPositions88[0].y + a->wheelPositions88[1].y - (a->wheelPositions88[2].y + a->wheelPositions88[3].y)) >> 3);
    a->zHistory144[SC->w34] = Atan2Table(a->width10C, (a->wheelPositions88[0].y + a->wheelPositions88[2].y - (a->wheelPositions88[1].y + a->wheelPositions88[3].y)) >> 3);
    a->pitchAverage164 += a->xHistory124[SC->w34];
    a->rollAverage166 += a->zHistory144[SC->w34];
    a->pitchAverage164 = (u16)a->pitchAverage164 - a->xHistory124[SC->w38];
    a->rollAverage166 = (u16)a->rollAverage166 - a->zHistory144[SC->w38];
    a->rot50 = (short)(a->pitchAverage164 / 10);
    a->rot58 = (short)(a->rollAverage166 / 10);
    a->pitchAverage168 += a->xHistory124[SC->w3c];
    a->rollAverage16A += a->zHistory144[SC->w3c];
    a->pitchAverage168 -= a->xHistory124[SC->w40];
    a->rollAverage16A -= a->zHistory144[SC->w40];
    a->ang80 = a->pitchAverage168 / 10 + (a->renderAngleX1AC >> 1);
    a->ang82 = *(u16 *)&a->rot54;
    a->ang84 = a->renderAngleZ1AE + a->rollAverage16A / 10;
    a->ir70[0] += a->pos10[0];
    a->ir70[1] += a->pos10[1];
    a->ir70[2] += a->pos10[2];
    a->pos10[0] += a->ir40[0];
    a->pos10[1] += a->ir40[1];
    a->pos10[2] += a->ir40[2];
    a->ir70[0] += a->ir40[0];
    a->ir70[1] += a->ir40[1];
    {
        /* ASM_BARRIER: pin w78 to v0 */
        register int w78 asm("$2") = a->ir70[2];
        int w48 = a->ir40[2];
        int w44 = a->ir40[1];
        int w10 = a->pos10[0];
        int w18 = a->pos10[2];
        int wcc;
        int v2;
        /* ASM_BARRIER: schedule w10/w18 loads before wcc load */
        __asm__ ("" : "+r"(w78) : "r"(w10), "r"(w18), "r"(w48), "r"(w44));
        wcc = a->bodyPosition.y;
        w78 += w48;
        wcc += w44;
        a->ir70[2] = w78;
        /* ASM_BARRIER: schedule w78 store before wcc store */
        __asm__ volatile("" : "+r"(wcc));
        a->bodyPosition.y = wcc;
        v2 = w10;
        /* ASM_BARRIER: force addu v1, a2, zero */
        __asm__ ("" : "+r"(v2));
        v2 >>= 2;
        a->position20.words[0] = v2;
        a->sampleCount16C += 1;
        v2 = w18;
        /* ASM_BARRIER: force addu v1, a3, zero */
        __asm__ volatile("" : "+r"(v2));
        v2 >>= 2;
        a->bodyPosition.x = w10;
        a->bodyPosition.z = w18;
        a->position20.words[2] = v2;
        a->position20.words[1] = a->pos10[1] >> 2;
    }
}

void SetTrackNodeData(char *a)
{
    struct TrackHeader4 sp;
    int i;
    TrackNodeGrid = (short *)(a + 4);
    sp = *(struct TrackHeader4 *)a;
    TrackNodes = (TrackSample3C *)(a + 0x808);
    TrackLength = 0;
    TrackNodeCount = ((int *)a)[0x201];
    for (i = 0; i < TrackNodeCount; i++) {
        TrackLength += TrackNodes[i].segmentLength1C;
    }
    TrackLengthKilometers = TrackLength / 1024;
    D_800ACC74 = (TrackLength >> 8) + 1;
}

