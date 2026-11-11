#include "game/sound/sndvab.h"
/* RODATA 0x800104c4 0x2c */
#include "rr/ui.h"
#include "rr/load_cursor.h"
#include "rr/badge_rows.h"
#include "rr/menu_text_state.h"
#include "rr/audio_loop.h"
#include "game/data/ldscn1.h"
#include "game/gfx/fadectl.h"
#include "game/gfx/fadein.h"
#include "game/gfx/fadesel.h"
#include "game/menu/menutran.h"
#include "game/race/racegrid.h"
#include "game/race/ranking.h"
#include "game/system/modepad.h"
#include "game/menu/menusave.h"
#include "game/system/modeinit.h"
#include "rr/audio_cd_controller.h"
#include "rr/driver_control.h"
#include "rr/frame_environment.h"
#include "game/gfx/gfxotag.h"
#include "game/data/globals.h"
#include "game/data/selection.h"
#include "game/data/screenframe.h"
#include "game/sound/fxplay.h"
#include "game/ui/uibar.h"
#include "game/menu/mnrows.h"
#include "game/ui/uidraw.h"
#include "game/demo/attract.h"

#include "rr/cd_directory_buffer.h"
extern char StrMainMenu0Log[];
extern short FadeCountdown;
extern int ActivePlayerIndex;
extern short PlayerCourseChoices[];
extern int DrawSubOtFlag;
extern short LinkHostFlag;
extern short ReverseCourse;
extern int MaskBitSetting;
extern unsigned short MainMenuUnlockBitmask;
extern short ModeIndex;
extern short MenuScriptId;
extern short ModeGroup;
extern int DialogueSceneMode;
#include "rr/menu_arena_base.h"
extern u16 SubmodeState;
extern short LinkState;
extern int ModeInitResetCount;
extern char StrInitMainMenu0[];
extern char LinkEntryState[];
extern int VSyncThreshold;
extern short MenuCursor;
extern short ActiveSceneId;
extern int ModeFrame;
extern int MenuReturnSubmodeIndex;

void TitleModeEnter(void);
void TitleMenuInit(void);
void TitleMenuUpdate(void);

void TitleModeEnter(void) {
    short *a0;
    char *a1;
    int a2;
    SetDispMask(0);
    SeqMuteVolume();
    a2 = 0;
    a1 = SelectionRows.bytes;
    a0 = PlayerCourseChoices;
    LinkHostFlag = -1;
    LinkState = 0;
    FadeCountdown = 0;
    ActivePlayerIndex = 0;
    ModeInitResetCount = 0;
    ReverseCourse = 0;
    do {
        a0[0] = 0;
        a0[1] = 0;
        a1[3] = 0;
        a1 += 4;
        a2++;
        a0 += 2;
    } while (a2 < 4);
    DialogueSceneMode = 2;
    MenuScriptId = 4;
    ModeGroup = 1;
    ModeIndex = 0x23;
    SubmodeState = 0;
    LoadCursor = CdDirectoryBuffer;
    MenuArenaBase = CdDirectoryBuffer;
    if ((u16)((u16)GameModeType - 2) < 3U) BackupSaveData();
    GameModeType = 0;
    _sio_control(1, 1, 0);
    DrawSubOtFlag = 0;
    MaskBitSetting = 0;
    ModeSelectBaseCount = (MainMenuUnlockBitmask & 1) + 7;
    DebugPrintStub(StrMainMenu0Log);
}

void TitleMenuInit(void)
{
  int i;
  int y;
  int v;
  short c7;
  SetupDrawDispEnvs(0, 0, 0, 3);
  c7 = 7;
  i = 0;
  ModeFrame = 0;
  ScreenFrame = 0;
  VSyncThreshold = 0x80;
  MainMenuChoice = 0;
  MenuCursor = -1;
  LinkEntryState[3] = 0;
  LinkEntryState[2] = 0;
  LinkEntryState[1] = 0;
  LinkEntryState[0] = 0;
  ActiveSceneId = 0x104;
  while (i < (ModeSelectBaseCount + 1))
  {
    v = i;
    if (ModeSelectBaseCount == c7)
    {
      if (i == ModeSelectBaseCount)
      {
        v = 8;
      }
    }
    InitUiRow(i, 0x6C, (i * 0x22) + 0x40, 0x54, 0x20, 0, 0, 1, 0x7F, v);
    i++;
  }

  if (CountProgressFlags() == 0x140)
  {
    ExtraModeUnlockedFlag = 1;
  }
  CardDialogueStatePtr->b7 = 2;
  InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
  BadgeRows->half02 = 1;
  _sio_control(1, 1, 0);
  DebugPrintStub(StrInitMainMenu0);
}

void TitleMenuUpdate(void)
{
    int s0;
    int v1;
    int a1;
    int s0b;
    int c3, c4;
    BadgeRow1C *a0;

    ModeFrame = ModeFrame + 1;
    ScreenFrame = ScreenFrame + 1;
    s0 = RunIntroScriptStep();
    switch (SubmodeState) {
    case 0:
    case 10:
        _sio_control(1, 1, 0);
        if (UnlockNoticeMask > 0) {
            UpdateBadgeRowAnim(0);
            if (BadgeRows->state10 == 3) {
                if (MenuReturnSubmodeIndex != 0 || SubmodeState == 0 || CardDialogueStatePtr->st == 4) {
                    if (SubmodeState == 0xA) {
                        PlaySfx(3);
                        CardDialogueStatePtr->b4 = CardDialogueStatePtr->b4 + 1;
                    } else {
                        goto set_submode;
                    set:
                        CardDialogueStatePtr->b4 = v1;
                        goto call;
                    set_submode:
                        SubmodeState = 0xA;
                    }
                    v1 = CardDialogueStatePtr->b4;
                    if (v1 < 4) {
                        c3 = 3;
                        c4 = 4;
                        a1 = UnlockNoticeMask;
                        a0 = BadgeRows;
                        do {
                            if ((a1 >> v1) & 1)
                                goto set;
                            if (v1 == c3)
                                a0->state10 = c4;
                            v1++;
                        } while (v1 < 4);
                    }
                }
            call:
                UpdateDialogue(BadgeRows->x08 + 0xA, BadgeRows->y0A + 0x28, CardDialogueStatePtr);
                return;
            } else if (BadgeRows->state10 == 0xB) {
                SubmodeState = 1;
                UnlockNoticeMask = 0;
                ScreenFrame = 0;
                return;
            }
        } else {
            SubmodeState = 1;
            ScreenFrame = 0;
            return;
        }
        return;
    case 1:
        _sio_control(1, 1, 0);
        if (Pad0Buttons & 0x800) {
            if (s0 == 0)
                ResetDemoFadeState();
            PlaySfx(1);
            EnableDemoFadeSkip();
            MenuCursor = 0;
            ResetLinkSioControl();
            SubmodeState = 2;
            InitBadgeRow(0, 0x10, 0x1B6, 0x40, 0x22, 0, 0xF00);
            BadgeRows->half02 = 1;
        }
        if (ScreenFrame >= 0x2EEU)
            SubmodeState = 5;
        return;
    case 2:
        ResetLinkSioControl();
        s0b = MenuCursor;
        if (UiRows->byte14 == 2) {
            if (ModeSelectMenuStep() != 0) {
                SubmodeState = 3;
                FadeCountdown = 0x40;
                ResetScreenCounters();
                StartDemoFadeIn(MainMenuChoice);
                BadgeRows->state10 = 4;
            }
        }
        if (s0b != MenuCursor)
            ScreenFrame = 0;
        if (ScreenFrame >= 0xBB8U)
            SubmodeState = 5;
        DrawModeSelectRows();
        UpdateBadgeRowAnim(0);
        return;
    case 3:
        if (FadeCountdown <= 0) {
            if (LoadOpeningMovieTimStep() == 1) {
                SubmodeState = 0;
                ModeGroup = PendingModeId >> 8;
                ModeIndex = PendingModeId & 0xFF;
            }
        } else {
            FadeCountdown = FadeCountdown - 1;
        }
        DrawModeSelectRows();
        UpdateBadgeRowAnim(0);
        return;
    case 5:
        _sio_control(1, 1, 0);
        if (AttractCycleCounter & 1) {
            ModeGroup = 2;
            ModeIndex = 3;
            SubmodeState = 0;
        } else {
            StartAttractDemo(0);
        }
        AttractCycleCounter = AttractCycleCounter + 1;
        if (AttractCycleCounter >= 0x64)
            AttractCycleCounter = 0;
        return;
    }
}
