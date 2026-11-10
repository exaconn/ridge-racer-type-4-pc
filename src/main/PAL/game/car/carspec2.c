#include "game/sound/sndque.h"
#include "rr/overlay661_init.h"
#include "rr/audio_state_types.h"
#include "game/car/carspec2.h"
#include "rr/audio_loop.h"
typedef struct { u8 reserved00[0xF8]; int f0, f4; unsigned short a[6]; unsigned short b[6]; short c, d; u8 reserved11C[0x320 - 0x11C]; } T;

typedef struct {
    short x, y;
} CurvePt;

extern T CarSpecControlPool[];
extern CurvePt RubberBandCurve[];
extern short BonusCarUnlockMask;
extern RaceStandingsTable *RaceStandings;
extern int ReplayState;
extern int RaceState;

void AverageCarSpec(int x, int y, int z);
int CalcRubberBandFactor(RaceVehicle *a0, int a1);

void AverageCarSpec(int x, int y, int z) {
    int i;
    T *tx = &CarSpecControlPool[x];
    T *ty = &CarSpecControlPool[y];
    T *tz = &CarSpecControlPool[z];
    tx->f0 = (ty->f0 + tz->f0) / 2;
    for (i = 0; i < 6; i++) {
        tx->a[i] = (ty->a[i] + tz->a[i]) >> 1;
    }
    for (i = 0; i < 6; i++) {
        tx->b[i] = (ty->b[i] + tz->b[i]) >> 1;
    }
    tx->c = (ty->c + tz->c) / 2;
    tx->d = (ty->d + tz->d) / 2;
}

typedef struct {
    s32 a[2];
    s32 b[2];
    s32 unk10[2];
    RaceStandingEntry0C entries18[8];
} RaceStandingsArrayView;

int CalcRubberBandFactor(RaceVehicle *a0, int a1) {
    int s1 = 0;
    RaceStandingsArrayView *s2 = (RaceStandingsArrayView *)RaceStandings;
    RaceVehicleControl *t0 = &a0->control;
    int d;
    int i;
    int lo;
    int b;
    int v2;
    int v1;
    int v0;

    if (s2->b[a1] >= RubberBandCurve[0].x) {
        for (i = 1; i < 7; i++) {
            if (s2->b[a1] < RubberBandCurve[i].x) {
                break;
            }
        }
        if (i < 4) {
            d = s2->b[a1] - RubberBandCurve[i].x;
            lo = ((RubberBandCurve[i].y - RubberBandCurve[i - 1].y) * d) / (RubberBandCurve[i].x - RubberBandCurve[i - 1].x);
            s1 = RubberBandCurve[i].y + lo;
            d = t0->lateralSpeed58 - s2->a[a1];
            if (d < 0) {
                d = 0;
            }
            if (d > a0->input) {
                d = a0->input;
            }
            if (a0->input > 0) {
                d = (d << 10) / a0->input;
            }
            if (d >= 0x100) {
                d = 0x400 - (((d - 0x100) * 4) / 3);
                s1 = (s1 * d) / 1024;
            }
            i = t0->boostTimer44;
            if (i > 0) {
                s1 = (s1 * (0x400 - ((i << 10) / __builtin_abs(t0->divisor11C)))) / 1024;
            }
        } else if (i < 7) {
            int lo2;
            int denom;
            d = s2->b[a1] - RubberBandCurve[i].x;
            denom = RubberBandCurve[i].x - RubberBandCurve[i - 1].x;
            lo2 = ((RubberBandCurve[i].y - RubberBandCurve[i - 1].y) * d) / denom;
            /* Note: extend denom past div so local_alloc does not combine denom with lo2 */
            /* ASM_BARRIER: extend denom past div so local_alloc does not combine denom with lo2 */
            __asm__ ("" : : "r"(denom));
            s1 = RubberBandCurve[i].y + lo2;
            s1 += 0x800;
        }
        if (GameModeType == 1 || GameModeType == 6) {
            if (RaceStandings->entries18[0].unk00 == 0 && RaceStandings->entries18[1].unk00 == (s16)a0->flags1E4 && s2->b[a1] > BonusCarUnlockMask) {
                s1 = 0xC00;
            }
        }
    }
    if (ReplayState != 2 && RaceState < 4) {
        if (s2->b[a1] < 0x7D0) {
            if (SoundState.unk1A0C[((s16)a0->flags1E4 & 0xF) - 1] == 0) {
                if (SoundState.unk1A28 == 0) {
                    v0 = PsyqRand() & 1;
                    SoundState.unk1A2C = v0;
                    lo = v0 + 0xD;
                    SoundQueueEnqueue(lo, 0);
                }
                SoundState.unk1A0C[((s16)a0->flags1E4 & 0xF) - 1] = 1;
                SoundState.unk1A28 = 0x4B;
            }
        }
        if (s2->b[a1] >= 0x1771) {
            if (SoundState.unk1A0C[((s16)a0->flags1E4 & 0xF) - 1] != 0) {
                SoundState.unk1A0C[((s16)a0->flags1E4 & 0xF) - 1] = 0;
            }
        }
        if (RaceStandings->entries18[0].unk00 == (s16)a0->flags1E4 && RaceStandings->entries18[1].unk00 == 0) {
            if (s2->b[a1] >= -0x5DB) {
                if (SoundState.unk1A08 == 0) {
                    if (SoundState.unk1A28 == 0) {
                        v0 = PsyqRand() & 1;
                        SoundState.unk1A30 = v0;
                        SoundQueueEnqueue(v0 + 0xB, 0);
                    }
                    SoundState.unk1A08 = 1;
                    SoundState.unk1A28 = 0x4B;
                }
            }
            if (s2->b[a1] < -0x1388) {
                if (SoundState.unk1A08 != 0) {
                    SoundState.unk1A08 = 0;
                }
            }
        }
    }
    return s1;
}
