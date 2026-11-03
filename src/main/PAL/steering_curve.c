#include "rr/steering.h"

/* Build a two-phase rate curve in twelve-bit fixed-point units. */
void func_8004D354(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate)
{
    s32 half = duration / 2;
    s32 scaledDisplacement = displacement * 4096;
    s32 middleRate;
    s32 scaledInitialRate = initialRate * 4096;
    curve->initialRate = scaledInitialRate;
    middleRate = scaledDisplacement / half - (initialRate * 2048);
    curve->displacement = scaledDisplacement;
    curve->duration = duration;
    curve->middleRate = middleRate;
    curve->firstAcceleration = (middleRate - curve->initialRate) / half;
    curve->secondAcceleration = -middleRate / half;
}

s32 func_8004D420(const RrCurve24 *curve, s32 time, s32 *output)
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
