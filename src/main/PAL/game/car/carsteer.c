#include "game/car/car.h"

typedef struct {
    char pad0[0xB];
    u8 unkB;
} UnkF3A08;

extern s32 AngleDiffSigned(s16, s32);
extern s32 Atan2Table(s16, s32);
extern s32 rsin(s32);
extern UnkF3A08 *CarBoundsTable[];

int UpdateCarSteering(Car *arg0) {
    s32 s1;
    s32 s2;
    s32 s3;
    s32 v0;
    s32 i;

    s3 = 0;
    s2 = (arg0->unk1DA - arg0->unk1D8) * 4;
    if (arg0->unk1E4 & 0x10) {
        s1 = AngleDiffSigned(arg0->unk1D4, arg0->unk54) * 4;
        if (arg0->sub1F4.unk1C->unkA != 0) {
            s3 = (rsin((arg0->sub1F4.unk54 << 5) & 0xFE0) * arg0->unk1DE) / 8128;
        }
    } else {
        s1 = AngleDiffSigned(arg0->unk1D6, arg0->unk54) * 4;
    }

    if ((v0 = arg0->unk1D8 < 1000)) {
        s1 = (s1 * v0) / 1800;
    } else {
        s1 = (s1 * 1000) / 1800;
    }

    /* ASM_BARRIER: increase s2 ref count so global.c allocates $s2 before $s3 */
    __asm__("" : : "r"(s2));
    if (s2 < -0x10) {
        arg0->unk1DE -= 0x10;
    } else if (s2 > 0x10) {
        arg0->unk1DE += 0x10;
    }
    arg0->unk1DE = (arg0->unk1DE * 26) / 32;

    if (s1 < -0x20) {
        arg0->unk1DC -= 0x20;
    } else if (s1 > 0x20) {
        arg0->unk1DC += 0x20;
    }
    arg0->unk1DC = (arg0->unk1DC * 24) / 32;

    for (i = 0; i < 4; i++) {
        if (arg0->unk1BC[i] == 0) {
            arg0->unkE4[i] += (i & 1) ? -arg0->unk1DC : arg0->unk1DC;
            arg0->unkE4[i] += (i >= 2) ? -arg0->unk1DE : arg0->unk1DE;
        } else if (arg0->unk1BC[i] > 0) {
            arg0->unkE4[i] -= 8;
        }

        if (arg0->unkE4[i] > 0x100) {
            arg0->unkE4[i] = 0x100;
        } else if (arg0->unkE4[i] < -0x300) {
            arg0->unkE4[i] = -0x300;
        }
        arg0->unkE4[i] = (arg0->unkE4[i] * 24) / 32;
    }

    if (arg0->unk1DE > 0) {
        arg0->unkE4[2] += s3;
        arg0->unkE4[3] += s3;
    }

    s1 = 4 - CarBoundsTable[(s16)arg0->unk1E6 >> 1]->unkB;
    if (s1 <= 0) {
        s1 = 4;
    }

    arg0->unk1AC = Atan2Table(arg0->unk10E, ((arg0->unkE4[0] + arg0->unkE4[1]) - (arg0->unkE4[2] + arg0->unkE4[3])) >> 4) >> 3;
    arg0->unk1AE = (s16)(Atan2Table(arg0->s10C, ((arg0->unkE4[0] + arg0->unkE4[2]) - (arg0->unkE4[1] + arg0->unkE4[3])) >> 4) >> 2) / s1;

    if (arg0->unk1AE > 0x20) {
        arg0->unkDC[0] += arg0->unk1DC / 20;
        arg0->unkDC[2] += arg0->unk1DC / 16;
    } else if (arg0->unk1AE < -0x20) {
        arg0->unkDC[1] += (-arg0->unk1DC) / 20;
        arg0->unkDC[3] += (-arg0->unk1DC) / 16;
    }

    for (i = 0; i < 4; i++) {
        arg0->unkDC[i] = (arg0->unkDC[i] * 22) / 32;
        if (arg0->unkDC[i] <= 0) {
            arg0->unkDC[i] = 0;
        } else if (arg0->unkDC[i] >= (arg0->unk108 << 3)) {
            arg0->unkDC[i] = arg0->unk108 << 3;
        }
    }

    arg0->unk1A8 = Atan2Table(arg0->unk10E, ((arg0->unkDC[0] + arg0->unkDC[1]) - (arg0->unkDC[2] + arg0->unkDC[3])) >> 5);
    arg0->unk1AA = Atan2Table(arg0->s10C, ((arg0->unkDC[0] + arg0->unkDC[2]) - (arg0->unkDC[1] + arg0->unkDC[3])) >> 5);
}
