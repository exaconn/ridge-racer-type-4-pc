#include "rr/menu_text_rows.h"
#include "rr/overlay_menu.h"

extern s16 MenuCursor;
extern s16 MenuItemCount;
extern s32 ovl661_result_labels[22];
extern s32 TotalRaceTime;
extern s16 ovl661_finish_ranks[2];
extern s16 ovl661_player_standings[2];
extern s16 ovl661_player_win_counts[2];
extern s32 ModeFrame;

extern void SetMenuFrameOptions(s32, s32, s32);

void ovl661_init_results(void) {
    s32 i;

    InitMenuTextRows((MenuTextRow60 *)SharedWorkArena);
    MenuCursor = 0;
    MenuItemCount = 3;
    for (i = 0; i < 11; i++) {
        SetMenuTextRow(i, i, 0, 0, ovl661_result_labels[i]);
    }
    for (i = 11; i < 22; i++) {
        SetMenuTextRow(i, i - 11, 0, 0, -1);
    }
    for (i = 33; i < 44; i++) {
        SetMenuTextRow(i, i - 33, 2, 0, ovl661_result_labels[i - 22]);
    }
    if (TotalRaceTime >= 0) {
        if (ovl661_finish_ranks[0] > ovl661_finish_ranks[1]) {
            ovl661_player_standings[0] = 1;
            ovl661_player_standings[1] = 0;
        } else {
            ovl661_player_standings[0] = 0;
            ovl661_player_standings[1] = 1;
        }
        if (ovl661_player_win_counts[ovl661_player_standings[0]] < 99) {
            ovl661_player_win_counts[ovl661_player_standings[0]]++;
        }
    }
    ModeFrame = 0;
    SetMenuFrameOptions(0, 1, 0);
}
