#include "rr/menu_selection.h"

s32 DecodeCourseIndex(s32 a0) {
    ReverseCourse = a0 >= 8;
    return CoursePreviewSceneIds[a0 & 7];
}
