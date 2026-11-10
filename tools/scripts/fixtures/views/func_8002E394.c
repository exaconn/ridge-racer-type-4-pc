#include "rr/race.h"

#define I(o) (*(int *)(a0 + (o)))

#define U(o) (*(unsigned short *)(a0 + (o)))

#define S(o) (*(short *)(a0 + (o)))

typedef struct {
    unsigned short halfW00, pad02, halfL04;
    s16 height06;
} VehicleBounds08;

extern short D_800AD508;

extern VehicleBounds08 *CarBoundsTable[];

void SetCarBounds(RaceVehicle *a)
{
    int n;
    VehicleBounds08 **pp;
    int b;
    if (D_800AD508 != 0) {
        n = a != PlayerCars;
    } else {
        n = 3;
        if (a == PlayerCars) {
            n = 2;
        }
    }
    a->slotIndex1E6 = n * 2;
    b = (int)CarBoundsTable;
    pp = (VehicleBounds08 **)(b + n * 4);
    a->height108 = (*pp)->height06 * 4;
    a->width10C = ((short)(*pp)->halfW00 + 5) * 2;
    a->halfSpan10E = 0x28 - (*pp)->halfL04;
    a->footprint110.fields.corners[0].x = -(((*pp)->halfW00 + 5) * 4);
    a->footprint110.fields.corners[0].y = -(((*pp)->halfL04 - 0x14) * 4);
    a->footprint110.fields.corners[1].x = ((*pp)->halfW00 + 5) * 4;
    a->footprint110.fields.corners[1].y = -(((*pp)->halfL04 - 0x14) * 4);
    a->footprint110.fields.corners[2].x = -(((*pp)->halfW00 + 5) * 4);
    a->footprint110.fields.corners[2].y = -0x50;
    a->footprint110.fields.corners[3].x = ((*pp)->halfW00 + 5) * 4;
    a->footprint110.fields.corners[3].y = -0x50;
}
