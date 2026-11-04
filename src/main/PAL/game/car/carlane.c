/* RODATA 0x80010fec 0x28 */
#include "types.h"
#include "game/car/grid.h"
#include "game/car/carbound.h"
#include "game/car/carlane.h"
#include "rr/audio_loop.h"
#include "rr/menu_choice.h"
#include "rr/menu_setup.h"
#include "rr/track_projection.h"

typedef struct {
    s32 baseSpeed00, gearCount04;
    u16 ratios08[6];
    u16 thresholds14[6];
    s16 maximum20, span22, divisor24, factor26;
    s32 altSpeed28;
    s16 steering2C, pad2E;
} Spec30;

typedef struct {
    u8 pad00[0x38];
    s16 laneTimer38, laneStart3A;
    u8 pad3C[4];
    s32 laneCooldown40;
    s16 boostTimer44;
    u8 pad46[0xF8 - 0x46];
    s32 maxSpeedF8, gearCountFC;
    s16 gearRatios100[6];
    u16 shiftThresholds10C[6];
    s16 maximum118, span11A, divisor11C, factor11E;
    s32 altSpeed120;
    s16 steering124;
    u8 pad126[4];
    s16 metric12A;
} CarSub1F4;

typedef struct {
    s32 posScaled00[3];
    u8 pad0C[0x20 - 0x0C];
    s32 pos20[3];
    u8 pad2C[0x50 - 0x2C];
    s32 rot50, rot54, rot58;
    u8 pad5C[0xF0 - 0x5C];
    s32 trackNodeF0;
    u8 padF4[0x172 - 0xF4];
    s16 lateralOffset172;
    u8 pad174[0x1D4 - 0x174];
    s16 heading1D4;
    u8 pad1D6[0x1E4 - 0x1D6];
    s16 flags1E4;
    u8 pad1E6[0x1F4 - 0x1E6];
    CarSub1F4 sub1F4;
} ComCar;

typedef struct {
    s16 grid00[8];
    s32 count10;
    s32 mode14;
    u8 pad18[0x53C - 0x18];
    Grid53C *p53C;
    Spec30 *p540;
    Spec30 *p544;
    Spec30 *p548;
} ContextAC650;

extern ContextAC650 D_800AC650;
extern ComCar D_80107198[];
extern Spec30 *D_800ACB94;
extern u16 D_8009FE76;

extern void func_80096434(void *, s32);
extern void func_8002E4F4(ComCar *);
extern s32 func_80026A74(s32 *, s32);
extern void func_8001E84C(const char *, s32);

void func_80045F48(s32 count) {
    s32 i, j, k;
    CarSub1F4 *sub;
    Spec30 *spec;
    s32 tmp;
    ComCar *car;

    car = D_80107198;
    D_800AC650.count10 = count;
    func_80096434(car, 0x2260);
    for (i = 0; i < count; i++) {
        sub = &car->sub1F4;
        spec = &D_800AC650.p540[i];
        func_8002E4F4(car);
        car->flags1E4 = i + 1;
        for (j = 0; j < 8; j++) {
            if (D_800AC650.grid00[j] == i + 1) {
                k = j;
                break;
            }
        }
        car->pos20[0] = D_800AC650.p53C->pos[k].x;
        car->pos20[1] = 0;
        car->pos20[2] = D_800AC650.p53C->pos[k].z;
        car->posScaled00[0] = car->pos20[0] << 8;
        car->posScaled00[2] = car->pos20[2] << 8;
        car->trackNodeF0 = D_800AC650.p53C->f0;
        car->rot50 = 0;
        car->rot58 = 0;
        car->rot54 = D_800AC650.p53C->rot[k];
        car->heading1D4 = car->rot54;
        car->trackNodeF0 = func_80026A74(car->pos20, car->trackNodeF0);
        func_8002E868(car);
        func_800468F0(car);
        sub->laneTimer38 = 10;
        sub->laneStart3A = car->lateralOffset172;
        sub->maxSpeedF8 = spec->baseSpeed00 << 8;
        sub->gearCountFC = spec->gearCount04;
        for (j = 0; j < 6; j++) {
            sub->gearRatios100[j] = spec->ratios08[j] << 2;
        }
        for (j = 0; j < 6; j++) {
            sub->shiftThresholds10C[j] = spec->thresholds14[j];
        }
        sub->maximum118 = spec->maximum20;
        sub->span11A = spec->span22;
        sub->divisor11C = spec->divisor24;
        sub->boostTimer44 = sub->divisor11C < 0 ? -sub->divisor11C : sub->divisor11C;
        sub->factor11E = spec->factor26;
        sub->altSpeed120 = spec->altSpeed28;
        sub->steering124 = spec->steering2C;
        sub->laneCooldown40 = (i * 25 + 50) / 2;
        sub->metric12A = 0;
        car++;
    }
    switch ((s16)D_800F4D44) {
    case 6:
        D_800AC650.mode14 = 1;
        D_800AC650.p544 = &D_800AC650.p540[D_800F3826 + 7];
        D_800AC650.p548 = 0;
        break;
    case 1:
        D_800AC650.mode14 = 1;
        D_800AC650.p544 = &D_800AC650.p540[D_800AD520 + 7];
        D_800AC650.p548 = 0;
        break;
    case 2:
    case 3:
    case 4:
        D_800AC650.mode14 = 2;
        D_800AC650.p544 = &D_800AC650.p540[7];
        D_800AC650.p548 = &D_800AC650.p540[7];
        break;
    case 5:
    default:
        D_800AC650.mode14 = 0;
        D_800AC650.p544 = &D_800AC650.p540[D_800AD520 + 7];
        break;
    }
    if ((s16)D_800F4D44 == 1) {
        for (i = 0; i < count; i++) {
            sub = &D_80107198[i].sub1F4;
            sub->metric12A = D_800AC650.p544->divisor24;
            tmp = sub->divisor11C * D_800AC650.p544->steering2C;
            if (tmp < 0) {
                tmp += 0x3FF;
            }
            sub->divisor11C = tmp >> 10;
            sub->boostTimer44 = sub->divisor11C < 0 ? -sub->divisor11C : sub->divisor11C;
        }
        D_8009FE76 = D_800ACB94->factor26;
    } else {
        D_8009FE76 = 0x320;
    }
    func_8001E84C("InitComCar OK!\n", i);
}
