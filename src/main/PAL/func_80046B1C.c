#include "types.h"
#include "rr/trig.h"
#include "rr/track_interpolation.h"

typedef struct {
    u8 pad00[0x58];
    s32 unk58;
    u8 pad5C[0x6E - 0x5C];
    s16 unk6E;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s32 unk84;
} Control1F4;

extern RrRaceStandingsTable *D_800AC738;
extern s8 D_80010F2C[][16];
extern s8 D_80010F8C[][16];
extern s16 D_800F4D44;

extern s32 func_80047DF8();
extern s32 func_80047CE0();

#define SIN(x) (((x) & 0x800) ? (((x) & 0x400) ? -D_800A68C0[0x400 - ((x) & 0x3FF)] : -D_800A68C0[(x) & 0x3FF]) : (((x) & 0x400) ? D_800A68C0[0x400 - ((x) & 0x3FF)] : D_800A68C0[(x) & 0x3FF]))

s32 func_80046B1C(RrRaceVehicle *arg0, s32 arg1) {
    s32 sp18;
    s32 sp1C;
    s32 s0;
    Control1F4 *s1;
    RrRaceStandingsTable *t;
    RrRaceVehicle *other;
    s32 i;
    s32 v1;
    s32 a2;
    s32 a1;
    s16 temp_v0;
    s32 v1_2;

    s0 = 0;
    t = D_800AC738;
    s1 = (Control1F4 *)((char *)arg0 + 0x1F4);
    other = t->entries18[arg1].vehicle08;

    i = -other->heading1D4 + 0x400;
    i -= D_800ABEA4[other->trackNodeF0].heading12;
    v1 = -((SIN(i) * other->input) / 4096);
    a1 = t->gap14;
    a2 = s1->unk58 - v1;
    a1 /= 100;
    if (a1 >= 16) {
        a1 = 15;
    }

    for (i = 0; i < 2; i++) {
        s0 += D_80010F2C[i][a1];
    }

    a2 = (a2 * 160) / 1168;
    if (a2 > 0) {
        if (a2 >= 80) {
            a2 = 79;
        }
        a2 /= 5;
        if (D_800F4D44 != 5) {
            s0 += D_80010F2C[2][a2];
        } else {
            s0 += D_80010F8C[2][a2];
        }
    } else {
        s0 -= 8;
    }

    s1->unk70 = func_80047DF8(arg0, 0, &s1->unk80, &s1->unk74, &s1->unk78);
    s1->unk7C = func_80047DF8(arg0, 10, &s1->unk84, &sp18, &sp1C);
    temp_v0 = func_80047CE0(arg0);
    s1->unk6E = temp_v0;
    /* ASM_BARRIER: keep sh 0x6E(s1) before the shift sequence */
    __asm__ volatile("");
    v1_2 = temp_v0 << 12;

    if (s0 > 0) {
        if (v1_2 < s1->unk7C) {
            s0 = 0;
        }
    }
    return s0;
}
