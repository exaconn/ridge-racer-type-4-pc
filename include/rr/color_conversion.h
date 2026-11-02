#ifndef RR_COLOR_CONVERSION_H
#define RR_COLOR_CONVERSION_H
#include "rr/layout.h"
typedef s16 RrHsvColor[3];
typedef u8 RrRgbColor[3];
/* Hue is signed degrees; saturation/value clamp to0..4096. One360-degree
 * turn is added for negative hue before signed reduction. A negative final
 * sector leaves output untouched; sector0 can retain a negative fraction.
 * Input covers three aligned halfwords, output three bytes when written.
 * Output may overlap writable input: all three inputs are captured first. */
void func_8004C138(const RrHsvColor input,RrRgbColor output);
RR_ASSERT_SIZE(RrHsvColor,6);
RR_ASSERT_SIZE(RrRgbColor,3);
#endif
