#ifndef RR_COURSE_CHOICES_H
#define RR_COURSE_CHOICES_H
#include "rr/layout.h"
#include "rr/load_cursor.h"
#include "rr/menu_glyph.h"
#include "rr/menu_selection.h"
#include "rr/menu_arena_types.h"
#include "rr/selection.h"
#include "rr/course_choice_types.h"
extern CourseChoice4 CourseChoiceTable[2][9];
#include "rr/menu_arena_state.h"
extern u8 *ImageRows;
void ResetScreenCounters(void);
/* Nine readable entries; glyph-1 terminates. The final glyph0x167 counts
 * as the ninth valid entry. */
s32 CountChoiceGlyphs(const CourseChoice4 *choices);
/* Fresh player0..3, two-way group selector and course0..8 are required.
 * Scene setup may replace roots and selections before the copies are made.
 * The published UI pointer supplies at least two writable bytes. */
void ovl660_init_preview(void);
void ovl661_reset_preview_selection(void);
void ovl667_func_80116BB4(void);
RR_ASSERT_SIZE(CourseChoice4,4);
RR_ASSERT_OFFSET(CourseChoice4,second,2);
RR_ASSERT_OFFSET(CourseChoice4,first,3);
#endif
