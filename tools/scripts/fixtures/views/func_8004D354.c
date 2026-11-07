#include "rr/steering.h"

/* Build a two-phase rate curve in twelve-bit fixed-point units. */
void BuildRateCurve(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate)
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
