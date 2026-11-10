/* RODATA 0x80010fec 0x28 */
#include "types.h"
#include "game/car/grid.h"
#include "game/car/carbound.h"
#include "game/car/carlane.h"
#include "rr/audio_loop.h"
#include "rr/menu_choice.h"
#include "rr/menu_setup.h"
#include "rr/track_projection.h"
#include "rr/bios_devices.h"
#include "game/course/trkclip.h"
#include "rr/cd_poll.h"

typedef struct {
    s16 grid00[8];
    s32 count10;
    s32 mode14;
    u8 reserved18[0x53C - 0x18];
    Grid53C *p53C;
    RaceSpecEntry30 *p540;
    RaceSpecEntry30 *p544;
    RaceSpecEntry30 *p548;
} ContextAC650;

extern ContextAC650 RaceContext;
extern RaceVehicle ComCars[];
extern RaceSpecEntry30 *D_800ACB94;
extern u16 D_8009FE76;

extern void ResetCarState(RaceVehicle *);

void InitComCars(s32 count) {
    s32 i, j, k;
    RaceVehicleControl *sub;
    RaceSpecEntry30 *spec;
    s32 tmp;
    RaceVehicle *car;

    car = ComCars;
    RaceContext.count10 = count;
    PsyqBzero(car, 0x2260);
    for (i = 0; i < count; i++) {
        sub = &car->control;
        spec = &RaceContext.p540[i];
        ResetCarState(car);
        car->flags1E4 = i + 1;
        for (j = 0; j < 8; j++) {
            if (RaceContext.grid00[j] == i + 1) {
                k = j;
                break;
            }
        }
        car->position20.words[0] = RaceContext.p53C->pos[k].x;
        car->position20.words[1] = 0;
        car->position20.words[2] = RaceContext.p53C->pos[k].z;
        car->posScaled00[0] = car->position20.words[0] << 8;
        car->posScaled00[2] = car->position20.words[2] << 8;
        car->trackNodeF0 = RaceContext.p53C->f0;
        car->rot50 = 0;
        car->rot58 = 0;
        car->rot54 = RaceContext.p53C->rot[k];
        car->heading1D4 = car->rot54;
        car->trackNodeF0 = FindTrackNode(car->position20.words, car->trackNodeF0);
        AssignCarModelSlot(car);
        UpdateCarTrackProjection(car);
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
        sub->steeringEnabled124 = spec->steering2C;
        sub->laneCooldown40 = (i * 25 + 50) / 2;
        sub->metric12A = 0;
        car++;
    }
    switch ((s16)GameModeType) {
    case 6:
        RaceContext.mode14 = 1;
        RaceContext.p544 = &RaceContext.p540[D_800F3826 + 7];
        RaceContext.p548 = 0;
        break;
    case 1:
        RaceContext.mode14 = 1;
        RaceContext.p544 = &RaceContext.p540[D_800AD520 + 7];
        RaceContext.p548 = 0;
        break;
    case 2:
    case 3:
    case 4:
        RaceContext.mode14 = 2;
        RaceContext.p544 = &RaceContext.p540[7];
        RaceContext.p548 = &RaceContext.p540[7];
        break;
    case 5:
    default:
        RaceContext.mode14 = 0;
        RaceContext.p544 = &RaceContext.p540[D_800AD520 + 7];
        break;
    }
    if ((s16)GameModeType == 1) {
        for (i = 0; i < count; i++) {
            sub = &ComCars[i].control;
            sub->metric12A = RaceContext.p544->divisor24;
            tmp = sub->divisor11C * RaceContext.p544->steering2C;
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
    DebugPrintStub("InitComCar OK!\n", i);
}
