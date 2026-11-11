#include "rr/overlay666.h"
#include "rr/frame_storage_types.h"

void ovl666_init_results(void) {
    s32 i;
    s32 base;

    /* The selected label bank is0..2; the two progress selectors are0..3. */
    InitMenuTextRows((MenuTextRow60 *)SharedWorkArena);
    MenuCursor = 0;
    MenuItemCount = 3;
    if (ovl666_result_kind == 0) {
        SaveProgress.prefix.flags5C[SelectionRows.choice.bytes[1]] = 1;
        SaveProgress.prefix.progress
            .progress10[SelectionRows.choice.bytes[0]][SelectionRows.choice.bytes[1]] |= 0x80000;
    }
    base = ovl666_result_kind * 11;
    for (i = 0; i < 11; i++) {
        SetMenuTextRow(i, i, 0, 0, ovl666_result_labels[i + base]);
    }
    for (i = 11; i < 22; i++) {
        SetMenuTextRow(i, i - 11, 0, 0, -1);
    }
    for (i = 33; i < 44; i++) {
        SetMenuTextRow(i, i - 33, 2, 0, ovl666_result_labels[i - 11]);
    }
    SetMenuFrameOptions(0, 1, 0);
    ModeFrame = 0;
    ScreenFrame = 0;
}

s32 ovl666_update_retry_choice(s32 mode) {
    SetMenuTextRowStyle(MenuCursor + 37, 0, 0);
    StepMenuValue(0, 2, (char *)&MenuCursor, 3, 0);
    SetMenuTextRowStyle(MenuCursor + 37, 1, 1);
    if (MenuReturnSubmodeIndex) {
        PlaySfx(1);
        return MenuCursor;
    }
    return -1;
}

void ovl666_update_results(void) {
    s32 group;
    s32 i;
    s16 timer;
    s32 ready;
    MenuTextRow60 *rows;

    ModeFrame = (ModeFrame + 1U);
    ScreenFrame++;
    switch (SubmodeState) {
    case 0:
        if (TotalRaceTime == -1) {
            group = 1;
            if (MenuTextRows[11].signedBytes[2] >= 2) {
                SubmodeState = 3;
            }
        } else {
            group = 0;
            if (MenuTextRows[5].signedBytes[3] >= 21) {
                SetMenuFrameOptions(1, 1, 0);
                if (MenuReturnSubmodeIndex != 0) {
                    PlaySfx(1);
                    SubmodeState = 3;
                    SetMenuFrameOptions(0, 1, 0);
                }
            }
            if (ovl666_result_kind == 0) {
                MenuTextRows[7].marquee.at28[(ScreenFrame >> 2) % 22U] = 0x20;
                MenuTextRows[7].marquee.at28[((ScreenFrame >> 2) + 1U) % 22U] = 0x5B;
                MenuTextRows[7].marquee.at28[((ScreenFrame >> 2) + 2U) % 22U] = 0x5D;
                MenuTextRows[7].marquee.at4[((ScreenFrame >> 2) + 2U) % 22U] = 0x40;
            }
        }
        break;
    case 3:
        group = 3;
        if (MenuTextRows[38].signedBytes[3] >= 12) {
            SetMenuFrameOptions(1, 1, 0);
            if (ovl666_update_retry_choice(0) >= 0) {
                ResetScreenCounters();
                InitMenuArenas();
                FadeCountdown = 64;
                SubmodeState = MenuCursor + 4;
            }
        }
        break;
    case 4:
        SetMenuFrameOptions(0, 1, 0);
        timer = (u16)FadeCountdown - 1;
        FadeCountdown = timer;
        AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.tags, (64 - timer) * 4, 69);
        group = 3;
        if (FadeCountdown <= 0) {
            ovl666_result_kind = 1;
            MenuScriptId = 0;
            ModeGroup = 1;
            ModeIndex = 35;
            SubmodeState = 0;
        }
        break;
    case 5:
        SetMenuFrameOptions(0, 1, 0);
        timer = (u16)FadeCountdown - 2;
        FadeCountdown = timer;
        AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.tags, (64 - timer) * 4, 69);
        group = 3;
        if (FadeCountdown <= 0) {
            group = -1;
            SubmodeState = 8;
        }
        break;
    case 6:
        SetMenuFrameOptions(0, 1, 0);
        timer = (u16)FadeCountdown - 1;
        FadeCountdown = timer;
        AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.tags, (64 - timer) * 4, 69);
        group = -1;
        if (FadeCountdown <= 0) {
            ModeGroup = 1;
            ModeIndex = 3;
            SubmodeState = 0;
        }
        break;
    case 8:
        ready = LoadResultsSceneStep();
        if (ready == 1) {
            SeqStartTrack(0);
            ovl666_init_menu();
            ModeGroup = ready;
            ModeIndex = 79;
            SubmodeState = 0;
        }
        return;
    default:
        break;
    }
    UpdateScreenFade(0x1000C);
    DrawResultsStandingsOverlay(ModeFrame);
    if (group >= 0) {
        i = 0;
        do {
            UpdateMenuTextRow(i + group * 11);
            i++;
        } while (i < 11);
    }
    rows = MenuTextRows;
    if ((rows[0].bytes[2] - 3) < 5U || (rows[11].bytes[2] - 3) < 5U) {
        DrawResultsRankIcons();
    }
}
