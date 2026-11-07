#include "types.h"

#include "rr/course_choice_types.h"

void UnpackChoiceGlyph(signed char *a0, int a1) {
    int t = a1 / 160;
    a0[0] = t;
    a0[1] = a1 / 40 - t * 4;
    a0[2] = a1 % 40;
}
