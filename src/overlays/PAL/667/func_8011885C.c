#include "rr/menu_text_rows.h"
#include "rr/frame_storage_types.h"

extern s32 ModeFrame, TotalRaceTime, D_800F3A60, ActivePlayerIndex;
extern u16 SubmodeState;
extern s16 ModeGroup, ModeIndex, FadeCountdown, MenuCursor, LinkHostFlag;
extern s16 LinkState, D_8010C068, LinkPlayerCount;
extern s16 LinkFinishCarOrder[4];
extern s16 LinkPlayerWinCounts[4];
extern char LinkPlayerNames[4][12];

s32 ExitOnLinkLost(void);
s32 UpdateLinkHandshake(s32);
void BeginLinkSync(void);
void SetMenuFrameOptions(s32, s32, s32);
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
s32 ovl667_update_retry_choice(s32);
void ovl667_init_car_selection(s32);

void ovl667_update_results(void) {
    s32 pressed;
    s32 group;
    s32 i;
    s32 result;
    s32 player;

    ModeFrame++;
    if (ExitOnLinkLost() != 0) {
        return;
    }
    pressed = UpdateLinkHandshake(2);
    if (ModeFrame >= 0x08000000U) {
        ModeFrame = 3;
    }
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
        if (MenuTextRows[3].signedBytes[3] >= 16) {
            SetMenuFrameOptions(1, 1, group);
            if (D_800F3A60 != 0) {
                SubmodeState = 3;
                SetMenuFrameOptions(group, 1, group);
            }
        }
        FormatDecimalDigits(MenuTextRows[4].marquee.at28 + 8, LinkFinishCarOrder[0] + 1, 0);
        CopyStringPadSpaces(MenuTextRows[5].marquee.at28 + 1, LinkPlayerNames[LinkFinishCarOrder[0]], 9);
        /* The original literal occupies 12 bytes (zero word before "WIN "). */
        CopyStringClearTail(MenuTextRows[5].marquee.at28 + 10, "WON !!!\0\0\0");
        for (i = 0; i < 4; i++) {
            if (i < LinkPlayerCount) {
                player = LinkFinishCarOrder[i];
                CopyStringPadSpaces(MenuTextRows[7 + i].marquee.at28 + 5, LinkPlayerNames[player], 9);
                CopyStringClearTail(MenuTextRows[7 + i].marquee.at28 + 14, "WIN ");
                FormatDecimalDigits(MenuTextRows[7 + i].marquee.at28 + 18, LinkPlayerWinCounts[player], 1);
            } else {
                MenuTextRows[7 + i].marquee.at28[0] = 0;
            }
        }
        break;
    case 3:
        group = 3;
        if (MenuTextRows[39].signedBytes[3] >= 17) {
            SetMenuFrameOptions(1, 1, 0);
            if (ovl667_update_retry_choice(0) >= 0) {
                ResetScreenCounters();
                InitMenuArenas();
                FadeCountdown = 64;
                SubmodeState = MenuCursor + 5;
                BeginLinkSync();
                SetMenuFrameOptions(0, 1, 0);
            }
        }
        break;
    case 4:
        group = -1;
        if (pressed == 0 && FadeCountdown == 64) {
            break;
        }
        FadeCountdown--;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            D_8010C068 = 1;
            ModeGroup = 1;
            ModeIndex = 41;
            SubmodeState = 0;
            LinkState = 0;
        }
        break;
    case 5:
        group = 3;
        if (pressed == 0 && FadeCountdown == 64) {
            CopyStringClearTail(MenuTextRows[38].marquee.at28, "*PLEASE WAIT! ");
            break;
        }
        FadeCountdown -= 2;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            group = -1;
            SubmodeState = 8;
        }
        break;
    case 6:
        group = -1;
        FadeCountdown--;
        AddScreenFadeTiles((u32 *)(&CurrentDrawBuf->buffer.ordering70.words[0]), (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            ModeGroup = 1;
            ModeIndex = 3;
            SubmodeState = 0;
        }
        break;
    case 8:
        result = LoadResultsSceneStep();
        if (result != 1) {
            return;
        }
        SeqStartTrack(0);
        LinkState = 0;
        ActivePlayerIndex = (result - LinkHostFlag) * 2;
        ovl667_init_car_selection(ActivePlayerIndex);
        ModeGroup = result;
        ModeIndex = 60;
        SubmodeState = 0;
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
