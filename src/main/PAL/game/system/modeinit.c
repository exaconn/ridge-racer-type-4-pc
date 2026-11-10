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

extern int CdDirectoryBuffer;
extern char StrMainMenu0Log[];
extern short D_800AC5D0;
extern int D_800AC5D4;
extern short PlayerCourseChoices[];
extern int DrawSubOtFlag;
extern short D_800AD508;
extern short ReverseCourse;
extern int MaskBitSetting;
extern unsigned short D_800F3974;
extern short D_800F3A26;
extern short MenuScriptId;
extern short ModeGroup;
extern int D_800F4CA8;
extern int MenuArenaBase;
extern u16 D_800F4D8A;
extern short LinkState;
extern int D_8010BFB8;
extern char StrInitMainMenu0[];
extern char LinkEntryState[];
extern int VSyncThreshold;
extern short MenuCursor;
extern short ActiveSceneId;
extern int ModeFrame;
extern int D_800F3A60;
extern void PlaySfx();

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
    a1 = SelectionRows;
    a0 = PlayerCourseChoices;
    D_800AD508 = -1;
    LinkState = 0;
    D_800AC5D0 = 0;
    D_800AC5D4 = 0;
    D_8010BFB8 = 0;
    ReverseCourse = 0;
    do {
        a0[0] = 0;
        a0[1] = 0;
        a1[3] = 0;
        a1 += 4;
        a2++;
        a0 += 2;
    } while (a2 < 4);
    D_800F4CA8 = 2;
    MenuScriptId = 4;
    ModeGroup = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    LoadCursor = CdDirectoryBuffer;
    MenuArenaBase = CdDirectoryBuffer;
    if ((u16)((u16)GameModeType - 2) < 3U) BackupSaveData();
    GameModeType = 0;
    _sio_control(1, 1, 0);
    DrawSubOtFlag = 0;
    MaskBitSetting = 0;
    D_800F4CD0 = (D_800F3974 & 1) + 7;
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
  D_800F4D2C = 0;
  MenuCursor = -1;
  LinkEntryState[3] = 0;
  LinkEntryState[2] = 0;
  LinkEntryState[1] = 0;
  LinkEntryState[0] = 0;
  ActiveSceneId = 0x104;
  while (i < (D_800F4CD0 + 1))
  {
    v = i;
    if (D_800F4CD0 == c7)
    {
      if (i == D_800F4CD0)
      {
        v = 8;
      }
    }
    InitUiRow(i, 0x6C, (i * 0x22) + 0x40, 0x54, 0x20, 0, 0, 1, 0x7F, v);
    i++;
  }

  if (CountProgressFlags() == 0x140)
  {
    D_800F3970 = 1;
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
    switch (D_800F4D8A) {
    case 0:
    case 10:
        _sio_control(1, 1, 0);
        if (D_800F4BE0 > 0) {
            UpdateBadgeRowAnim(0);
            if (BadgeRows->state10 == 3) {
                if (D_800F3A60 != 0 || D_800F4D8A == 0 || CardDialogueStatePtr->st == 4) {
                    if (D_800F4D8A == 0xA) {
                        PlaySfx(3);
                        CardDialogueStatePtr->b4 = CardDialogueStatePtr->b4 + 1;
                    } else {
                        if (0) {
                        set:
                            CardDialogueStatePtr->b4 = v1;
                            goto call;
                        }
                        D_800F4D8A = 0xA;
                    }
                    v1 = CardDialogueStatePtr->b4;
                    if (v1 < 4) {
                        c3 = 3;
                        c4 = 4;
                        a1 = D_800F4BE0;
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
                D_800F4D8A = 1;
                D_800F4BE0 = 0;
                ScreenFrame = 0;
                return;
            }
        } else {
            D_800F4D8A = 1;
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
            D_800F4D8A = 2;
            InitBadgeRow(0, 0x10, 0x1B6, 0x40, 0x22, 0, 0xF00);
            BadgeRows->half02 = 1;
        }
        if (ScreenFrame >= 0x2EEU)
            D_800F4D8A = 5;
        return;
    case 2:
        ResetLinkSioControl();
        s0b = MenuCursor;
        if (UiRows->byte14 == 2) {
            if (ModeSelectMenuStep() != 0) {
                D_800F4D8A = 3;
                D_800AC5D0 = 0x40;
                ResetScreenCounters();
                StartDemoFadeIn(D_800F4D2C);
                BadgeRows->state10 = 4;
            }
        }
        if (s0b != MenuCursor)
            ScreenFrame = 0;
        if (ScreenFrame >= 0xBB8U)
            D_800F4D8A = 5;
        DrawModeSelectRows();
        UpdateBadgeRowAnim(0);
        return;
    case 3:
        if (D_800AC5D0 <= 0) {
            if (LoadOpeningMovieTimStep() == 1) {
                D_800F4D8A = 0;
                ModeGroup = PendingModeId >> 8;
                D_800F3A26 = PendingModeId & 0xFF;
            }
        } else {
            D_800AC5D0 = D_800AC5D0 - 1;
        }
        DrawModeSelectRows();
        UpdateBadgeRowAnim(0);
        return;
    case 5:
        _sio_control(1, 1, 0);
        if (D_8010BFBC & 1) {
            ModeGroup = 2;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        } else {
            StartAttractDemo(0);
        }
        D_8010BFBC = D_8010BFBC + 1;
        if (D_8010BFBC >= 0x64)
            D_8010BFBC = 0;
        return;
    }
}
