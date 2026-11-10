#ifndef RR_MENU_SELECTION_H
#define RR_MENU_SELECTION_H
#include "rr/layout.h"
extern u8 D_8009F64C[8];
extern s16 ReverseCourse,D_800AC648,CourseIndex;
extern s32 D_800AC5D4;
/* Word result0..255: the original loads the byte with zero extension. */
s32 DecodeCourseIndex(s32 selection);
typedef struct {s16 first,second;} MenuGridPoint;
extern MenuGridPoint PlayerCourseChoices[4];
RR_ASSERT_SIZE(MenuGridPoint,4);
RR_ASSERT_OFFSET(MenuGridPoint,second,2);
#endif
