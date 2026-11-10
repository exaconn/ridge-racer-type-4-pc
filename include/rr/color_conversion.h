#ifndef RR_COLOR_CONVERSION_H
#define RR_COLOR_CONVERSION_H
#include "rr/layout.h"
typedef s16 HsvColor[3];
typedef u8 RgbColor[3];
/* Hue is signed degrees; saturation/value clamp to0..4096. One360-degree
 * turn is added for negative hue before signed reduction. A negative final
 * sector leaves output untouched; sector0 can retain a negative fraction.
 * Input covers three aligned halfwords, output three bytes when written.
 * Output may overlap writable input: all three inputs are captured first. */
void HsvToRgb(const HsvColor input,RgbColor output);
RR_ASSERT_SIZE(HsvColor,6);
RR_ASSERT_SIZE(RgbColor,3);
#endif
