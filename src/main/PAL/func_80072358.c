#include "types.h"

typedef struct {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s16 f1C;
    s16 f1E;
    s16 f20;
    s16 f22;
} Node24;

typedef struct {
    s32 pad[9];
    s32 f24;
    s32 f28;
    s32 f2C;
} Node30;

typedef struct {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s16 f1C;
    s16 f1E;
    s16 f20;
    s16 f22;
    s32 f24;
    s32 f28;
} Node2C;

extern Node24 *D_8010ECAC;
extern Node30 *D_8010ECB0;
extern Node2C *D_8010ECB4;

s32 func_80072358(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 *sp = (s32 *)0x1F80007C;
    s32 d1, w, d2;

    sp[14] = arg2;
    sp[10] = arg4;
    sp[11] = arg5;
    sp[12] = arg6;
    sp[13] = arg7;

    if ((D_8010ECAC[arg1].f08 % 10) == 8 &&
        (arg1 + 1) < (D_8010ECB0[arg0].f28 + D_8010ECB0[arg0].f24) &&
        (D_8010ECB4[arg0].f00 + 10) > (D_8010ECAC[arg1].f00 * 6)) {
        d1 = (D_8010ECAC[arg1 + 1].f00 - D_8010ECAC[arg1].f00) * 6;
        w = D_8010ECAC[arg1].f00 * 6 - 10;
        w = D_8010ECB4[arg0].f00 - w;
        sp[0] = (D_8010ECAC[arg1].f0C - D_8010ECAC[arg1 - 1].f0C) / sp[11];
        sp[1] = (D_8010ECAC[arg1].f10 - D_8010ECAC[arg1 - 1].f10) / sp[11];
        sp[2] = (D_8010ECAC[arg1].f14 - D_8010ECAC[arg1 - 1].f14) / sp[11];
        sp[3] = (D_8010ECAC[arg1 + 1].f0C - D_8010ECAC[arg1].f0C) / d1;
        sp[4] = (D_8010ECAC[arg1 + 1].f10 - D_8010ECAC[arg1].f10) / d1;
        sp[5] = (D_8010ECAC[arg1 + 1].f14 - D_8010ECAC[arg1].f14) / d1;
        sp[6] = ((sp[0] * (20 - w)) >> 1) + ((sp[3] * w) >> 1);
        sp[7] = ((sp[1] * (20 - w)) >> 1) + ((sp[4] * w) >> 1);
        sp[8] = ((sp[2] * (20 - w)) >> 1) + ((sp[5] * w) >> 1);
        D_8010ECB4[arg0].f0C += sp[6] / 10;
        D_8010ECB4[arg0].f10 += sp[7] / 10;
        D_8010ECB4[arg0].f14 += sp[8] / 10;
    } else if ((arg1 - 1) >= D_8010ECB0[arg0].f24 &&
               (D_8010ECAC[arg1 - 1].f08 % 10) == 8 &&
               (D_8010ECB4[arg0].f00 - 10) <= (D_8010ECAC[arg1 - 1].f00 * 6)) {
        d2 = (D_8010ECAC[arg1 - 1].f00 - D_8010ECAC[arg1 - 2].f00) * 6;
        sp[0] = (D_8010ECAC[arg1].f0C - D_8010ECAC[arg1 - 1].f0C) / sp[11];
        sp[1] = (D_8010ECAC[arg1].f10 - D_8010ECAC[arg1 - 1].f10) / sp[11];
        sp[2] = (D_8010ECAC[arg1].f14 - D_8010ECAC[arg1 - 1].f14) / sp[11];
        sp[3] = (D_8010ECAC[arg1 - 1].f0C - D_8010ECAC[arg1 - 2].f0C) / d2;
        sp[4] = (D_8010ECAC[arg1 - 1].f10 - D_8010ECAC[arg1 - 2].f10) / d2;
        sp[5] = (D_8010ECAC[arg1 - 1].f14 - D_8010ECAC[arg1 - 2].f14) / d2;
        sp[6] = ((sp[0] * (sp[10] + 9)) >> 1) + ((sp[3] * (11 - sp[10])) >> 1);
        sp[7] = ((sp[1] * (sp[10] + 9)) >> 1) + ((sp[4] * (11 - sp[10])) >> 1);
        sp[8] = ((sp[2] * (sp[10] + 9)) >> 1) + ((sp[5] * (11 - sp[10])) >> 1);
        D_8010ECB4[arg0].f0C += sp[6] / 10;
        D_8010ECB4[arg0].f10 += sp[7] / 10;
        D_8010ECB4[arg0].f14 += sp[8] / 10;
    } else {
        D_8010ECB4[arg0].f0C = ((((((D_8010ECAC[arg1].f0C - D_8010ECAC[arg1 - 1].f0C) / sp[14]) * (100 - D_8010ECAC[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13] + D_8010ECAC[arg1 - arg3].f0C;
        D_8010ECB4[arg0].f10 = ((((((D_8010ECAC[arg1].f10 - D_8010ECAC[arg1 - 1].f10) / sp[14]) * (100 - D_8010ECAC[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13] + D_8010ECAC[arg1 - arg3].f10;
        D_8010ECB4[arg0].f14 = ((((((D_8010ECAC[arg1].f14 - D_8010ECAC[arg1 - 1].f14) / sp[14]) * (100 - D_8010ECAC[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13] + D_8010ECAC[arg1 - arg3].f14;
    }

    D_8010ECB4[arg0].f1C = D_8010ECAC[arg1 - arg3].f1C + ((((((D_8010ECAC[arg1].f1C - D_8010ECAC[arg1 - 1].f1C) / sp[14]) * (100 - D_8010ECAC[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13];
    D_8010ECB4[arg0].f1E = D_8010ECAC[arg1 - arg3].f1E + ((((((D_8010ECAC[arg1].f1E - D_8010ECAC[arg1 - 1].f1E) / sp[14]) * (100 - D_8010ECAC[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13];
    D_8010ECB4[arg0].f20 = D_8010ECAC[arg1 - arg3].f20 + ((((((D_8010ECAC[arg1].f20 - D_8010ECAC[arg1 - 1].f20) / sp[14]) * (100 - D_8010ECAC[arg1].f04)) / 100) * sp[10] * sp[12]) / sp[11]) / sp[13];

    if (D_8010ECAC[arg1].f04 != 0) {
        if ((D_8010ECAC[arg1].f08 % 10) == 4 || (D_8010ECAC[arg1].f08 % 10) == 5) {
            D_8010ECB4[arg0].f0C += ((((D_8010ECAC[arg1].f0C - D_8010ECAC[arg1 - 1].f0C) / sp[14]) * sp[10]) / sp[11]) * D_8010ECAC[arg1].f04 / 100;
            D_8010ECB4[arg0].f10 += ((((D_8010ECAC[arg1].f10 - D_8010ECAC[arg1 - 1].f10) / sp[14]) * sp[10]) / sp[11]) * D_8010ECAC[arg1].f04 / 100;
            D_8010ECB4[arg0].f14 += ((((D_8010ECAC[arg1].f14 - D_8010ECAC[arg1 - 1].f14) / sp[14]) * sp[10]) / sp[11]) * D_8010ECAC[arg1].f04 / 100;
            D_8010ECB4[arg0].f1C += ((((D_8010ECAC[arg1].f1C - D_8010ECAC[arg1 - 1].f1C) / sp[14]) * sp[10]) / sp[11]) * D_8010ECAC[arg1].f04 / 100;
            D_8010ECB4[arg0].f1E += ((((D_8010ECAC[arg1].f1E - D_8010ECAC[arg1 - 1].f1E) / sp[14]) * sp[10]) / sp[11]) * D_8010ECAC[arg1].f04 / 100;
            D_8010ECB4[arg0].f20 += ((((D_8010ECAC[arg1].f20 - D_8010ECAC[arg1 - 1].f20) / sp[14]) * sp[10]) / sp[11]) * D_8010ECAC[arg1].f04 / 100;
        }
    }
}
