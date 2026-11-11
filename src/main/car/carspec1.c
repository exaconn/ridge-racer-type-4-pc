#include "rr/race.h"
#include "rr/player_cars.h"
#include "game/car/carspec1.h"

/* RaceContext.draftSpec544 is the spec the blend moves toward. Every caller
 * passes target 0, which selects it (draftSpec548 is the second slot). */

RaceVehicleControl *CarSpecControl(s32 index) {
    return &ComCars[index].control;
}

void BlendCarSpec(int a0, int a1, int a2, int a3) {
    RaceSpecEntry30 *t3 = &RaceContext.carSpecs540[a0];
    RaceVehicleControl *t2 = CarSpecControl(a1);
    RaceSpecEntry30 *target = RaceContext.draftSpec544;
    int lo, v, i, a;
    if (a2 < 0x401) {
        lo = a2 * t2->factor11E;
        v = lo >> 10;
        if (lo < 0) v = (lo + 0x3FF) >> 10;
    } else {
        v = a2 - 0x800;
    }
    a = t3->baseSpeed00;
    t2->maxSpeedF8 = ((((target->baseSpeed00 - a) * v) / 1024) + a) << 8;
    i = 0;
    do {
        t2->gearRatios100[i] = ((((target->ratios08[i] - t3->ratios08[i]) * v) / 1024) + t3->ratios08[i]) * 4;
        i++;
    } while (i < 6);
    i = 0;
    do {
        t2->shiftThresholds10C[i] = t3->thresholds14[i] + (((target->thresholds14[i] - t3->thresholds14[i]) * v) / 1024);
        i++;
    } while (i < 6);
    t2->maximum118 = (unsigned short)t3->maximum20 + (((target->maximum20 - t3->maximum20) * v) / 1024);
    t2->span11A = (unsigned short)t3->span22 + (((target->span22 - t3->span22) * v) / 1024);
}
