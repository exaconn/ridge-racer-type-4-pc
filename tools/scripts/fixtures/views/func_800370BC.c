#include "types.h"

#include "rr/course_choice_types.h"

s32 func_800370BC(const RrCourseChoice4 *a0) {
    int i;
    for (i = 0; i < 9; i++) {
        if (a0[i].glyph == -1) break;
        if (i == 8 && a0[8].glyph != 0x167) break;
    }
    return i;
}
