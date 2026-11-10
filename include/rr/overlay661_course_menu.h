#ifndef RR_OVERLAY661_COURSE_MENU_H
#define RR_OVERLAY661_COURSE_MENU_H
#include "game/system/pad.h"
#include "rr/ui.h"
#include "rr/load_cursor.h"
#include "rr/icon_sprites.h"
#include "rr/tables.h"
#include "rr/selection_types.h"
#include "rr/menu_selection_word.h"
#include "rr/menu_arena_types.h"
extern s32 ScreenFrame,ModeFrame,NumLaps,MenuAvailableCourseMask,ovl661_available_course_count;
extern s16 SelectedCourseChoice,MenuCursor,MenuItemCount,FadeCountdown,CourseIndex,ModeIndex,MenuScriptId,ModeGroup,GameModeType;
extern s16 ovl661_finish_ranks[2];
extern s16 ComDifficultyLevel;
extern u16 SubmodeState;
extern MenuSelectionWord32 NumCars;
extern SelectionRows32 SelectionRows;
#include "rr/menu_arena_state.h"
extern u8 ovl661_difficulty_parameters[4];
void SetModelTexturePage(s32);
void AddScreenFadeTiles(u32*,s32,u32);
void UpdateScreenFade(s32);
void SetMenuFrameOptions(s32,s32,s32);
void DrawMenuStepArrows(s32,s32,s32,s32,s32,s32,s32);void StepMenuValue(s32,s32,char*,s32,s32);
s32 DecodeCourseIndex(s32);
void DrawCourseInfoPanel(s32,s32);
void DrawCourseMapPreview(s32,s32);
void ResetScreenCounters(void);
s32 LoadCarRangeStep(s32,s32);
s32 LoadTrackNodesStep(void);
void DrawUiRow(s32);
void PlaySfx(s32);
void SeqFadeVolume(s32,s32);
void SeqMuteVolume(void);
void ovl661_init_preview(s32);
/* UI roots cover at least26 complete rows. Highlight count is0..10; its byte span may extend beyond cells[8]
 * into later bytes of the same complete row.
 * Parameter table indices are0..3. Menu phase0 accepts selection0..3; phase5
 * with row24 state2 requires selection0..1 before each indexed access,
 * including after services. Confirmation sets selection2 together with row
 * state3, leaving that indexed branch. AC59C owns a complete word edited by
 * width4 input; signed low-half reads use its union member. Input .a owns the
 * resident aliasF3A60. Services may replace roots and valid live selectors. */
void ovl661_update_menu(void);
RR_ASSERT_SIZE(SelectionRows32,32);
#endif
