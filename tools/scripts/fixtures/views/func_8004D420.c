#include "rr/steering.h"

s32 EvalRateCurve(const Curve24 *curve, s32 time, s32 *output)
{
    s32 half;
    s32 rate;
    if (time >= curve->duration) {
        *output = -(curve->secondAcceleration / 2);
        return -1;
    }
    half = curve->duration / 2;
    if (time < half) {
        rate = curve->initialRate + curve->firstAcceleration / 2 + curve->firstAcceleration * time;
    } else {
        time -= half;
        rate = curve->middleRate + curve->secondAcceleration / 2 + curve->secondAcceleration * time;
    }
    *output = rate;
    return 0;
}
