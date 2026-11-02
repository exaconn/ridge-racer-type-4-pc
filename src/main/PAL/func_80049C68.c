#include "types.h"
#include "rr/trig.h"
#include "rr/track_interpolation.h"

typedef struct {
    u8 pad00[0xF0];
    s32 trackNodeF0;
    u8 padF4[0x184 - 0xF4];
    s32 first, second;
    u8 pad18C[0x1D4 - 0x18C];
    s16 heading1D4, prevHeading1D6, input, prevInput1DA;
    u8 pad1DC[0x1E4 - 0x1DC];
    s16 flags1E4;
    u8 pad1E6[0x320 - 0x1E6];
} Vehicle320;

typedef struct {
    u8 pad00[0x58];
    s32 unk58;
} Control1F4;

extern Vehicle320 D_80107198[];
extern Vehicle320 *D_800FFC20[];
extern RrRaceStandingsTable *D_800AC738;
extern s32 D_800AC664;
extern s16 D_800AC59C;

#define SIN(x) (((x) & 0x800) ? (((x) & 0x400) ? -D_800A68C0[0x400 - ((x) & 0x3FF)] : -D_800A68C0[(x) & 0x3FF]) : (((x) & 0x400) ? D_800A68C0[0x400 - ((x) & 0x3FF)] : D_800A68C0[(x) & 0x3FF]))

void func_80049C68(void) {
    int i, j;
    Vehicle320 *v;
    Control1F4 *c;
    RrRaceStandingsTable *t;
    int a0;
    RrRaceStandingEntry0C tmp;

    t = D_800AC738;
    for (i = 0; i < 0xB; i++) {
        v = &D_80107198[i];
        c = (Control1F4 *)((char *)v + 0x1F4);
        if (v->flags1E4 > 0) {
            a0 = -v->heading1D4 + 0x400;
            a0 -= D_800ABEA4[v->trackNodeF0].heading12;
            c->unk58 = -((SIN(a0) * v->input) / 4096);
        }
    }

    if (D_800AC664 > 0) {
        v = &D_800ABEF8[0];
        a0 = -v->heading1D4 + 0x400;
        a0 -= D_800ABEA4[v->trackNodeF0].heading12;
        t->unk00 = -((SIN(a0) * v->input) / 4096);
    }

    if (D_800AC664 == 2) {
        v = &D_800ABEF8[1];
        a0 = -v->heading1D4 + 0x400;
        a0 -= D_800ABEA4[v->trackNodeF0].heading12;
        t->unk04 = -((SIN(a0) * v->input) / 4096);
    }

    for (i = 0; i < D_800AC59C; i++) {
        v = D_800FFC20[i];
        t->entries18[i].unk00 = i;
        t->entries18[i].totalDist04 = v->first + v->second;
        t->entries18[i].vehicle08 = (RrRaceVehicle *)v;
    }

    for (i = 1; i < D_800AC59C; i++) {
        tmp = t->entries18[i];
        for (j = i - 1; j >= 0 && t->entries18[j].totalDist04 < tmp.totalDist04; j--) {
            t->entries18[j + 1] = t->entries18[j];
        }
        t->entries18[j + 1] = tmp;
    }
}
