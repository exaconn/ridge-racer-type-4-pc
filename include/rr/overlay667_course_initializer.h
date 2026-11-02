#ifndef RR_OVERLAY667_COURSE_INITIALIZER_H
#define RR_OVERLAY667_COURSE_INITIALIZER_H
#include "rr/layout.h"
#include "rr/course_choice_types.h"
#include "rr/menu_choice_types.h"
#include "rr/selection.h"
#include "rr/player_status.h"
#include "rr/course_choices.h"
extern s16 D_8010C080[4],D_8010C0D0[2],D_8010C0D8[2];
extern RrSelectionRows D_800F4D98;
extern RrCourseChoice4 D_8010BFD8[2][2][9];
extern RrCourseChoice4 D_800ABE08[2][9];
extern char D_8010C088[4][12];
extern char *ovl667_D_80118E00[4];
extern s8 D_800ABEBC[2][2];
extern s32 D_800F3A30,D_800AC5D4,D_8010C078;
extern s16 D_800F3914,D_800AC648,D_800F3A26,D_8010C0B8,D_8010C0D4;
extern u16 D_800F4C6A,D_800F4D8A;
void func_80092CB8(s32);void func_8001ECE8(s32,s32,s16,u32);s32 func_80035024(s32);void func_8002A604(void);
void func_8002B75C(char*,char*);void ovl667_func_801161A8(s32);void func_80037040(s8*,s32);void func_80053BB4(s32);void func_8002AF8C(s32,s32,s32);
/* Four complete selection prefixes occupy bytes 0..15 of the 32-byte owner.
 * Each player base freshly read after a service is 0 or 2; mode is 0 or 1.
 * Name lookups are four native pointers to strings fitting twelve bytes.
 * Copies cover the full 144-byte two-snapshot/two-player/nine-course owner. */
void ovl667_func_80116918(void);
#endif
