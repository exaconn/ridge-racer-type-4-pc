#ifndef RR_COURSE_CHOICES_H
#define RR_COURSE_CHOICES_H
#include "rr/layout.h"
#include "rr/menu_glyph.h"
#include "rr/menu_selection.h"
#include "rr/menu_arena_types.h"
#include "rr/selection.h"
#include "rr/course_choice_types.h"
extern RrCourseChoice4 D_800ABE08[2][9];
extern RrMenuArenas D_800F4D70;
extern void *D_800ACC18;
extern u8 *D_800F2AB0;
void func_8003A198(void);
/* Nine readable entries; glyph-1 terminates. The final glyph0x167 counts
 * as the ninth valid entry. */
s32 func_800370BC(const RrCourseChoice4 *choices);
/* Fresh player0..3, two-way group selector and course0..8 are required.
 * Scene setup may replace roots and selections before the copies are made.
 * The published UI pointer supplies at least two writable bytes. */
void ovl660_func_801163C4(void);
void ovl661_func_80115F8C(void);
void ovl667_func_80116BB4(void);
RR_ASSERT_SIZE(RrCourseChoice4,4);
RR_ASSERT_OFFSET(RrCourseChoice4,second,2);
RR_ASSERT_OFFSET(RrCourseChoice4,first,3);
#endif
