#include "rr/menu_text_rows.h"
#include "rr/frame_storage_types.h"

extern s32 ModeFrame, TotalRaceTime, D_800F3A60;
extern u16 SubmodeState;
extern s16 ModeGroup, ModeIndex, FadeCountdown, MenuCursor, MenuScriptId;
extern s16 ovl661_player_standings[2];
extern s16 ovl661_player_win_counts[2];
extern char ovl661_player_names[2][12];
extern s16 ovl661_finish_ranks[2];
extern char ovl661_wins_suffix_label[];
extern char ovl661_win_count_prefix[];

void SetMenuFrameOptions(s32, s32, s32);
void PlaySfx(s32);
void CopyStringClearTail(char *, char *);
void CopyStringPadSpaces(char *, char *, s32);
void FormatDecimalDigits(char *, s32, s32);
void AddScreenFadeTiles(u32 *, s32, u32);
void ResetScreenCounters(void);
void InitMenuArenas(void);
void UpdateScreenFade(s32);
void DrawResultsStandingsOverlay(s32);
void UpdateMenuTextRow(s32);
void DrawResultsRankIcons(void);
s32 LoadResultsSceneStep(void);
void SeqStartTrack(s32);
s32 ovl661_update_retry_choice(s32);
void ovl661_init_preview(s32);

void ovl661_update_results(void) {
    s32 group;
    s32 i;
    s32 player;
    s32 ready;

    ModeFrame++;
    switch (SubmodeState) {
    case 0:
        if (TotalRaceTime == -1) {
            group = 1;
            if (MenuTextRows[11].signedBytes[2] >= 2) {
                SubmodeState = 3;
            }
            break;
        }
        group = 0;
        if (MenuTextRows[3].signedBytes[3] >= 17) {
            SetMenuFrameOptions(1, 1, group);
            if (D_800F3A60 != 0) {
                PlaySfx(1);
                SubmodeState = 3;
                SetMenuFrameOptions(group, 1, group);
            }
        }
        FormatDecimalDigits(MenuTextRows[4].marquee.at28 + 8, ovl661_player_standings[0] + 1, 0);
        CopyStringPadSpaces(MenuTextRows[5].marquee.at28 + 1, ovl661_player_names[ovl661_player_standings[0]], 9);
        CopyStringClearTail(MenuTextRows[5].marquee.at28 + 10, ovl661_wins_suffix_label);
        for (i = 0; i < 2; i++) {
            player = ovl661_player_standings[i];
            CopyStringPadSpaces(MenuTextRows[7 + i].marquee.at28 + 5, ovl661_player_names[player], 9);
            CopyStringClearTail(MenuTextRows[7 + i].marquee.at28 + 14, ovl661_win_count_prefix);
            FormatDecimalDigits(MenuTextRows[7 + i].marquee.at28 + 18, ovl661_player_win_counts[player], 1);
        }
        break;
    case 3:
        group = 3;
        if (MenuTextRows[39].signedBytes[3] >= 17) {
            SetMenuFrameOptions(1, 1, 0);
            if (ovl661_update_retry_choice(0) >= 0) {
                ResetScreenCounters();
                InitMenuArenas();
                FadeCountdown = 64;
                SubmodeState = MenuCursor + 4;
                SetMenuFrameOptions(0, 1, 0);
            }
        }
        break;
    case 4:
        FadeCountdown--;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (64 - FadeCountdown) * 4, 69);
        group = 3;
        if (FadeCountdown <= 0) {
            MenuScriptId = 0;
            ovl661_finish_ranks[0] = -1;
            ModeGroup = 1;
            ModeIndex = 35;
            SubmodeState = 0;
        }
        break;
    case 5:
        FadeCountdown -= 2;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (64 - FadeCountdown) * 4, 69);
        group = 3;
        if (FadeCountdown <= 0) {
            group = -1;
            SubmodeState = 8;
        }
        break;
    case 6:
        FadeCountdown--;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (64 - FadeCountdown) * 4, 69);
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
            ovl661_init_preview(0);
            ModeGroup = ready;
            ModeIndex = 56;
            SubmodeState = 0;
        }
        return;
    }
    UpdateScreenFade(0x1000C);
    DrawResultsStandingsOverlay(ModeFrame);
    if (group >= 0) {
        for (i = 0; i < 11; i++) {
            UpdateMenuTextRow(i + group * 11);
        }
    }
    if ((u8)(MenuTextRows[0].bytes[2] - 3) < 5 || (u8)(MenuTextRows[11].bytes[2] - 3) < 5) {
        DrawResultsRankIcons();
    }
}
