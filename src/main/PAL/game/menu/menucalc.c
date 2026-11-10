#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"
#include "game/gfx/gfxmtx.h"
#include "game/menu/menustat.h"
#include "rr/menu_background.h"
#include "rr/frame_environment.h"
#include "game/data/ldscn1.h"
#include "game/gfx/gfxotag.h"
#include "game/menu/menu8.h"
#include "game/menu/menucalc.h"
#include "game/system/pad.h"
#include "game/data/globals.h"
#include "game/data/screenframe.h"
#include "game/ui/uitext.h"
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "rr/menu_state_types.h"
#include "rr/menu_text_state.h"
#include "game/ui/badgespr.h"
#include "rr/trig.h"
#include "rr/screen_tiles.h"
#include "game/ui/badge1.h"
#include "game/ui/uibar.h"
#include "game/sound/cdmain.h"
#include "game/sound/fxplay.h"

extern unsigned char BadgeGlyphUvTable[][4];
extern signed char BadgeStripPatterns[][8];
extern unsigned char CourseSelectGroupCounts[];
extern unsigned char GpStageCourseBaseIndex[];
extern MenuSettings2C SelectionState;
extern unsigned char MakerTeamCarCounts[];
extern int ModeFrame;
extern int SetMenuFrameOptions();
extern int UpdateBadgeRowAnim();
extern int InitRect16Row();
extern short ModeIndex;
extern short ModeGroup;
extern int DialogueSceneMode;
extern unsigned short SubmodeState;
extern s32 VSyncThreshold;
extern s16 ActiveSceneId;
extern void ResetScreenCounters();
extern void PlaySfx();
extern unsigned short FadeCountdown;
extern BadgeRow1C *BadgeRows;
extern int D_800F4D74;

int DrawCarStatBars(int x, int a1);
int DrawStepProgressBar(int a0, int a1);
void DialogueIntroModeUpdate(void);
void DialogueSceneSetup(void);
void DialogueModeUpdate(void);
void AddScreenBackdropTile(void);

int DrawCarStatBars(int x, int a1)
{
  int s3 = 0x78;
  unsigned char (*new_var)[4];
  int i = 0;
  int s0;
  int s2;
  int a;
  do
  {
    s2 = a1 - (i * 0x32);
    if (s2 >= 0x81)
    {
      s2 = 0x80;
    }
    else
      if (s2 < 0)
    {
      s2 = 0;
    }
    s0 = SelectionState.phase17 >> 1;
    if (s0 >= 3)
    {
      s0 = 2;
    }
    if (i == 2)
    {
      s0 += 0xB;
      a = s0;
    }
    else
    {
      a = s0;
      if (i == 1)
      {
        s0 = SelectionState.phase17 + 3;
        a = s0;
      }
    }
    s3 += CourseSelectGroupCounts[i];
    x = AddBadgeSpriteStrip(a, &CurrentDrawBuf->buffer.ordering70.words[0], x, 0x20, s3, s2);
    if (i == 1)
    {
      unsigned char *t;
      int b = BadgeStripPatterns[s0][4];
      s0 = GpStageCourseBaseIndex[SelectionState.phase17] + 0x20;
      new_var = BadgeGlyphUvTable;
      t = new_var[s0];
      x = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.words[0], x, b + 0x34, s3, t[2], t[3], t[0], t[1], 0x4A, 0x2E28, i, s2);
    }
    i++;
  }
  while (i < 3);
  return x;
}

int DrawStepProgressBar(int a0, int a1) {
    int s0 = 0x28;
    int sel = D_800AD4EF;
    int i = 0;
    int v, y;
    do {
        s0 += MakerTeamCarCounts[i];
        if (sel == i) {
            v = rsin((ModeFrame << 6) & 0xFC0) / 64 + 0xBF;
        } else if (i < sel) {
            v = 0x80;
        } else {
            s0 += 8;
            goto next;
        }
        y = s0;
        s0 += 8;
        a1 = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, y, 0x190, 8, 0x1C, v, v, v);
next:
        i++;
    } while (i < 8);
    return a1;
}

void DialogueIntroModeUpdate(void)
{
    int t;
    if (ModeFrame <= 0x7FFFU) {
        ModeFrame += 1;
    }
    UpdateScreenFade(0x1000B);
    if (BadgeRows->state10 >= 4) {
        UpdateBadgeRowAnim(0);
    }
    if ((u8)UiRows->byte14 >= 3) {
        DrawUiRow(0);
    }
    if (ImageRows->f1 >= 3) {
        UpdateImageRow(0);
    }
    if (ModeFrame >= 3U) {
        DialogueSceneMode = 1;
        t = LoadDialogueStep();
        if (t == 1) {
            if (StepMenuCameraTransition(MENU_TRANSITION_ALL) == MENU_TRANSITION_ALL) {
                ModeFrame = 0;
                InitBadgeRow(0, 0xC, 0x130, 0x128, 0x6C, 0, 0);
                InitRect16Row(0, 0x10, 0x48, 0x120, 0x120, 0, 0);
                InitImageRow(0, -1, 0, 0);
                ModeGroup = t;
                ModeIndex = 0xC;
                SubmodeState = 0;
            }
        }
    } else {
        SetMenuFrameOptions(0, 1, 0);
    }
}

void DialogueSceneSetup(void) {
    SetupDrawDispEnvs(0, 0, 0, 2);
    ModeFrame = 0;
    VSyncThreshold = 0x80;
    ActiveSceneId = 0x10C;
}

void DialogueModeUpdate(void) {
    int s0;
    BadgeRow1C *tab;
    int a1;
    ModeFrame += 1;
    ScreenFrame += 1;
    if (UpdateImageRow(0) > 0) {
        UpdateDialoguePanel();
    }
    UpdateDialogueBox();
    if (Dialogue->st == 4 || (Pad0Buttons & 0x800)) {
        s0 = BadgeRows[Dialogue->s16].state10;
        if (s0 == 3) {
            if (PlayerControllers[0].action28 != 0 || (PlayerControllers[0].edges08.fields.pressed & 0x800)) {
                PlaySfx(1);
                SetMenuFrameOptions(0, 1, 0);
                tab = BadgeRows;
                tab[Dialogue->s16].state10 = 4;
                ImageRows->f1 = s0;
                FadeCountdown = 0;
                if (PendingModeId == 0x10F || PendingModeId == 0x12A || PendingModeId == 0x205) {
                    FadeCountdown = 0x40;
                    ResetScreenCounters(tab);
                    LoadCursor = D_800F4D74;
                    LoadTimListStep(0);
                    SeqFadeVolume(0, (short)(FadeCountdown - 4));
                    SubmodeState = 1;
                } else if (PendingModeId == 0x111) {
                    FadeCountdown = 0x40;
                    SeqFadeVolume(0, 0x3C);
                    SubmodeState = 1;
                }
            }
        }
    }
    if (SubmodeState == 1) {
        if ((short)FadeCountdown > 0) {
            FadeCountdown -= 1;
        }
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - (short)FadeCountdown) * 4, 0x45);
    }
    if (BadgeRows[Dialogue->s16].state10 == 0xB) {
        if (ImageRows->f1 == 5) {
            if (PendingModeId == 0x10F || PendingModeId == 0x12A) {
                if (LoadTimListStep(1) == 1) {
                    SeqMuteVolume();
                    if ((short)FadeCountdown <= 0) {
                        unsigned c = PendingModeId;
                        SubmodeState = 0;
                        ModeGroup = c >> 8;
                        ModeIndex = c & 0xFF;
                    }
                }
            } else if (PendingModeId == 0x205) {
                SeqMuteVolume();
                if ((short)FadeCountdown <= 0) {
                    unsigned c = PendingModeId;
                    SubmodeState = 0;
                    ModeGroup = c >> 8;
                    ModeIndex = c & 0xFF;
                }
            } else {
                if ((short)FadeCountdown <= 0) {
                    unsigned c = PendingModeId;
                    SubmodeState = 0;
                    ModeGroup = c >> 8;
                    ModeIndex = c & 0xFF;
                }
            }
        }
    }
    UpdateScreenFade(0x1000A);
}

void AddScreenBackdropTile(void) {
    int *g = (int *)SCRATCHPAD;
    int b = *g;
    *g = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[703], b, 0, 0, 0x140, 0x200, 0x20, 0x20, 0x20);
}
