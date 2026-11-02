#ifndef RR_OVERLAY666_PREVIEW_H
#define RR_OVERLAY666_PREVIEW_H
#include "rr/race.h"
#include "rr/projection_state.h"
#include "rr/course_choices.h"
#include "rr/ui.h"
#include "rr/badge_rows.h"
#include "rr/menu_selection_word.h"
#include "rr/menu_selection.h"
#include "rr/menu_glyph.h"
#include "rr/renderer_setup.h"
#include "rr/course_choice_types.h"
typedef RrCourseChoice4 RrExtraCourseChoice;
extern RrExtraCourseChoice ovl666_D_80116FA0[4][9];
extern s32 ovl666_D_80116F90;
extern RrMenuSelectionWord32 ovl666_D_80116F98;
extern s32 D_800F2DE4,D_800ABEB0;
extern s16 D_800AC64A,D_800AC5E0,D_800F3826;
extern void *D_800F4D40;
void func_8002AF8C(s32,s32,s32);
void func_8004F24C(s32);
void func_8002B330(s32,s32,char*,s32,s32);
void func_8002B12C(s32,s32,s32,s32,s32,s32,s32);
void func_8002A604(void);
void func_80037110(void);
void func_8002E4F4(RrRaceVehicle *vehicle);
/* Layout/vehicle helpers may replace arena and badge roots and selections.
 * Fresh row0..3 and column0..8 must be valid for each table access. */
void ovl666_func_80115A88(void);
/* Controller roots supply at least seven complete UI rows, one badge row,
 * one icon context, two vehicles and three readable mode bytes. Every live
 * course-table access requires category0..3 and selection.word0..8. Width2
 * input edits selection.halves[0]; the resulting full word must remain valid.
 * Menu choices are0..1; the course dispatch matrix requires primary/variant
 *0..3. Helpers may replace roots and selectors before subsequent accesses. */
void ovl666_func_80115C5C(void);
RR_ASSERT_SIZE(RrExtraCourseChoice,4);
#endif
