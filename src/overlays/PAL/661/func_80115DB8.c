#include "rr/diagnostics.h"
#include "rr/course_choices.h"
#include "rr/save_progress.h"
#include "rr/frame_environment.h"
extern s16 ovl661_player_win_counts[2],ovl661_selected_cars[2],ovl661_selected_groups[2];
extern s16 ComDifficultyLevel,NumCars,MenuCursor,MenuItemCount;
extern s32 NumLaps,ovl661_available_course_count;
extern CourseChoice4 ovl661_car_choices[2][2][9];
extern char ovl661_player_names[24],ovl661_player2_default_name[];
extern const char ovl661_scene_name[];
extern u16 ModeGroup;
extern s16 ModeIndex,SubmodeState;
void SetDispMask(s32 mode);
void ClearScreenOverlayState(void);
void CopyStringClearTail(char *destination,char *source);
void ovl661_init_preview(s32 mode);
void SeqStartTrack(s32 mode);
void SetMenuFrameOptions(s32,s32,s32);
/* Both destination name slots contain a NUL within12 bytes; the saved name
 * is terminated within its first12 bytes. String-tail cleanup keeps that bound. */
void ovl661_init_menu(void)
{
    s32 i;
    s32 j;

    SetDispMask(0);
    ClearScreenOverlayState();
    for (i = 0; i < 2; i++) {
        ovl661_selected_groups[i] = 0;
        ovl661_selected_cars[i] = 0;
        ovl661_player_win_counts[i] = 0;
    }
    ComDifficultyLevel = 0;
    SelectedCourseChoice = 0;
    CourseIndex = 0;
    NumLaps = 3;
    NumCars = 2;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 9; j++) {
            ovl661_car_choices[0][i][j] = CourseChoiceTable[i][j];
            ovl661_car_choices[1][i][j] = CourseChoiceTable[0][j];
        }
    }
    CopyStringClearTail(ovl661_player_names, (char *)SaveProgress.prefix.progress.prefix00.bytes);
    CopyStringClearTail(ovl661_player_names + 12, ovl661_player2_default_name);
    SetupDrawDispEnvs(0, 0, 0, 2);
    ovl661_init_preview(0);
    UnpackChoiceGlyph((s8 *)&SelectionRows, CourseChoiceTable[0][0].glyph);
    UnpackChoiceGlyph((s8 *)&SelectionRows + 4, CourseChoiceTable[0][0].glyph);
    SeqStartTrack(0);
    ovl661_available_course_count = 8;
    ModeGroup = 1;
    ModeIndex = 56;
    SubmodeState = 0;
    DebugPrintStub(ovl661_scene_name);
    SetMenuFrameOptions(0, 1, 1);
}
