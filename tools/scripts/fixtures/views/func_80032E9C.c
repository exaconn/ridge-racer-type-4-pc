#include "rr/menu_selection.h"

s32 DecodeCourseIndex(s32 a0) {
    ReverseCourse = a0 >= 8;
    return D_8009F64C[a0 & 7];
}
