/* RODATA 0x800106d0 0x1c */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"
#include "rr/ui.h"
#include "game/menu/menuwork.h"
#include "game/ui/barsel0.h"
#include "rr/audio_loop.h"
#include "rr/menu_background.h"
#include "game/data/globals.h"
#include "rr/race_setup.h"
#include "rr/audio_cd_controller.h"
#include "game/ui/badge1.h"
#include "game/ui/uitext.h"
#include "game/ui/badgeprim.h"
#include "game/ui/uibar.h"
#include "game/ui/uidraw.h"
#include "game/menu/mnchoice.h"
#include "game/menu/mnrows.h"
#include "game/data/ldscn1.h"

typedef struct {
    int x00;
    char x04;
    char x05;
    char x06;
    unsigned char b07;
    short x08;
    short y0A;
    short w0C;
    short h0E;
    int x10;
    int x14;
    int x18;
} GlyphRow1C;

extern unsigned char ResultMenuRowTable[];
extern unsigned char ResultMenuTableIndices[];
extern unsigned short SelectedCourseChoice;
extern int ModeFrame;
#include "rr/menu_arena_base.h"
extern signed char LoadState;
extern int GlyphRows;
extern s16 ModeIndex;
extern s16 ModeGroup;
extern s16 SubmodeState;
extern void SetMenuFrameOptions();
extern char NewCar0Msg[];
extern char StrNewCarLog[];
extern int MenuStepDirection;
extern int MenuReturnSubmodeIndex;
extern struct {
    short a;
    short b;
    short x04;
    short y06;
    short w08;
    short h0A;
    short u0C;
    short v0E;
    short h10;
} *BadgeRows;
extern struct {
    int x00;
    signed char b4;
    char b5;
    signed char b6;
    char d;
} *Dialogue;

void DrawResultsStandingsOverlay();
void InitResultsMenuRows(int a0);
void NewCarModeEnter(void);
void NewCarModeUpdate(void);

void DrawResultsStandingsOverlay(int a0) {
  int s4;
  int a1;
  int sz2;
  int new_var;
  int s3;
  int s1;
  int s2;
  int s0;
  int t0;
  int d;
  int u;
  unsigned char *p;
  unsigned char *q;
  unsigned char *base;
  s4 = 0x80;
  if (a0 < 0x41)
  {
    s4 = a0 * 2;
  }
  a1 = *((int *) SCRATCHPAD);
  s3 = 0x50;
  s1 = 0;
  do
  {
    if (s1 < 2)
    {
      d = GameModeType;
      if ((d != 1) && (s1 == 1))
      {
        goto next;
      }
      switch (d)
      {
        case 1:
          s2 = ResultMenuTableIndices[(MenuSubmodePendingId * 2) + s1];
          break;

        case 0:
          s2 = 3;
          break;

        case 2:

        case 3:
          s2 = 0;
          break;

        case 4:
          s2 = 1;
          break;

        case 6:
          s2 = 2;
          break;

      }

    }
    if (s1 == 2)
    {
      s2 = 0xC;
    }
    else
      if (s1 == 3)
    {
      int t = s2 + 1;
      s2 = t + (SelectedCourseChoice & 7);
    }
    p = ResultMenuRowTable + (s2 * 4);
    a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, s3, 0x48, p, 0x263A, (s4 + 1) * 2);
    u = s3 + 2;
    s3 = u + p[2];
    next:
    s1++;

  }
  while (s1 < 4);
  s2 = 0x16;
  if (GameModeType != 1)
  {
    s0 = 1;
    s3 = 0xD6;
  }
  else
  {
    s2 = 0x15;
    s0 = 2;
    s3 = 0xE6;
  }
  t0 = 0x263A;
  s1 = 0;
  if (s0 != 0)
  {
    base = ResultMenuRowTable;
    do
    {
      new_var = s3 - 2;
      a1 = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, s3, 0x24, (unsigned char *)((s2 * 4) + (int)base), t0, (s4 + 1) * 2);
      s2 = 0x17;
      s1++;
      q = base + 0x5C;
      { int t = new_var; s3 = t - q[2]; }
      t0 = 0x263B;
    }
    while (s1 < s0);
  }
  *((int *) SCRATCHPAD) = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], a1, 0x2E);
}

void InitResultsMenuRows(int a0) {
    int i = 0;
    int a;
    int t;
    GlyphRow1C *p;
    do {
        a = a0 + i;
        t = a * 0x1C;
        ((GlyphRow1C *)(t + GlyphRows))->x18 = ((GlyphRow1C *)(i * 0x1C + GlyphRows))->x18 + 0x30000;
        InitGlyphRow(a, -2, 0, 0);
        i += 1;
        p = (GlyphRow1C *)(t + GlyphRows);
        InitRect16Row(p->b07, p->x08, p->y0A, p->w0C, p->h0E, 0, 0);
    } while (i < 4);
    InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    InitUiRow(0, 0x28, 0x48, 0xC0, 0x20, 0, 0, 4, 3, 4);
    BadgeRows->b = 1;
    Dialogue->b4 = 1;
    Dialogue->d = 0;
    ModeFrame = 0;
    LoadState = 0;
    LoadCursor = MenuArenaBase;
}

void NewCarModeEnter(void) {
    UpdateScreenFade(0x10006);
    InitResultsMenuRows(0);
    ModeFrame = 0;
    VSyncThreshold = 0x80;
    ModeGroup = 1;
    ModeIndex = 0xA;
    SubmodeState = 0;
}

void NewCarModeUpdate(void) {
    int a;
    int t;
    int r;
    int s1;
    ModeFrame += 1;
    if (Dialogue->b4 >= 0x22) {
        DrawUiRow(0);
        a = 0x10006;
    } else {
        a = 0x10007;
    }
    UpdateScreenFade(a);
    if (UpdateResultsBarSelection(MenuStepDirection) != 0) {
        if (BadgeRows->h10 == 3) {
            StepResultsMenuChoice();
            return;
        }
    }
    if (Dialogue->b4 == 1) {
        if (MenuReturnSubmodeIndex != 0 || ModeFrame >= 0x1F5U) {
            t = Dialogue->b6;
            if (t == 4) {
                Dialogue->b4 = 0x22;
                BadgeRows->h10 = t;
                return;
            }
        }
    }
    if (MenuActiveDialogId == 2) {
        r = LoadCarRangeStep(0, 1);
        if (r == 1) {
            if (BadgeRows->h10 == 0xB) {
                SetMenuFrameOptions(0, 1, 0);
                DebugPrintStub(NewCar0Msg);
                s1 = LoadCursor;
                InitMenuArenas();
                InitCarPreviewScene();
                ModeGroup = r;
                ModeIndex = 0x22;
                SubmodeState = 0;
                LoadCursor = s1;
                DebugPrintStub(StrNewCarLog);
            }
        }
    }
}
