#ifndef RR_GAME_CAR_CARSPEC2_H
#define RR_GAME_CAR_CARSPEC2_H

#include "types.h"
#include "rr/race_types.h"

/* Rubber-band curve (0x8009FE70..0x8009FE8B): seven (distance, factor)
 * points. InitComCars stores the course's draft factor into point 1's
 * factor (0x8009FE76) and the GP leader test reads point 2's distance
 * (0x8009FE78); both are members of this one table. */
typedef struct {
    short x, y;
} CurvePt;
extern CurvePt RubberBandCurve[7];

void AverageCarSpec(int x, int y, int z);
int CalcRubberBandFactor(RaceVehicle *a0, int a1);

#endif /* RR_GAME_CAR_CARSPEC2_H */
