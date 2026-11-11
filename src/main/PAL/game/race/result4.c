#include "game/race/result4.h"
#include "rr/gpu_scratch_types.h"
#include "rr/badge_rows.h"
#include "rr/menu_text_state.h"
#include "game/ui/uipart1.h"
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


extern unsigned short SelectedCourseChoice;
extern int ModeFrame;
#include "rr/menu_arena_base.h"
extern signed char LoadState;
extern s16 ModeIndex;
extern s16 ModeGroup;
extern s16 SubmodeState;
extern char NewCar0Msg[];
extern char StrNewCarLog[];
extern int MenuStepDirection;
extern int MenuReturnSubmodeIndex;


void DrawResultsStandingsOverlay(int fadeLevel) {
  u32 intensity;
  void *cursor;
  int nextX;
  int x;
  int row;
  int label;
  int count;
  int clut;
  int mode;
  int advance;
  UiUvRect *labelUv;
  UiUvRect *separatorUv;
  intensity = 0x80;
  if (fadeLevel < 0x41)
  {
    intensity = (u32)fadeLevel * 2;
  }
  cursor = ((GpuScratchPointers *)SCRATCHPAD)->cursor;
  x = 0x50;
  for (row = 0; row < 4; row++)
  {
    if (row < 2)
    {
      mode = GameModeType;
      if ((mode != 1) && (row == 1))
      {
        continue;
      }
      switch (mode)
      {
        case 1:
          label = ResultMenuTableIndices[(MenuSubmodePendingId * 2) + row];
          break;

        case 0:
          label = 3;
          break;

        case 2:

        case 3:
          label = 0;
          break;

        case 4:
          label = 1;
          break;

        case 6:
          label = 2;
          break;

      }

    }
    if (row == 2)
    {
      label = 0xC;
    }
    else
      if (row == 3)
    {
      int courseLabelBase = label + 1;
      label = courseLabelBase + (SelectedCourseChoice & 7);
    }
    labelUv = ResultMenuRowTable + label;
    cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, x, 0x48, labelUv, 0x263A, (intensity + 1) * 2);
    advance = x + 2;
    x = advance + labelUv->w;
  }
  label = 0x16;
  if (GameModeType != 1)
  {
    count = 1;
    x = 0xD6;
  }
  else
  {
    label = 0x15;
    count = 2;
    x = 0xE6;
  }
  clut = 0x263A;
  row = 0;
  if (count != 0)
  {
    do
    {
      nextX = x - 2;
      cursor = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, x, 0x24, &ResultMenuRowTable[label], clut, (intensity + 1) * 2);
      label = 0x17;
      row++;
      separatorUv = &ResultMenuRowTable[23];
      x = nextX - separatorUv->w;
      clut = 0x263B;
    }
    while (row < count);
  }
  ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 0x2E);
}

void InitResultsMenuRows(int firstRow) {
    int rowIndex = 0;
    int destination;
    do {
        destination = firstRow + rowIndex;
        GlyphRows[destination].imageBase18 = GlyphRows[rowIndex].imageBase18 + 0x30000;
        InitGlyphRow(destination, -2, 0, 0);
        rowIndex += 1;
        InitRect16Row(GlyphRows[destination].glyph07, GlyphRows[destination].x08, GlyphRows[destination].y0A, GlyphRows[destination].w0C, GlyphRows[destination].h0E, 0, 0);
    } while (rowIndex < 4);
    InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    InitUiRow(0, 0x28, 0x48, 0xC0, 0x20, 0, 0, 4, 3, 4);
    BadgeRows->half02 = 1;
    Dialogue->b4 = 1;
    Dialogue->b7 = 0;
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
    u8 *savedCursor;
    ModeFrame = (u32)ModeFrame + 1;
    if (Dialogue->b4 >= 0x22) {
        DrawUiRow(0);
        a = 0x10006;
    } else {
        a = 0x10007;
    }
    UpdateScreenFade(a);
    if (UpdateResultsBarSelection(MenuStepDirection) != 0) {
        if (BadgeRows->state10 == 3) {
            StepResultsMenuChoice();
            return;
        }
    }
    if (Dialogue->b4 == 1) {
        if (MenuReturnSubmodeIndex != 0 || (u32)ModeFrame >= 0x1F5U) {
            t = Dialogue->st;
            if (t == 4) {
                Dialogue->b4 = 0x22;
                BadgeRows->state10 = t;
                return;
            }
        }
    }
    if (MenuActiveDialogId == 2) {
        r = LoadCarRangeStep(0, 1);
        if (r == 1) {
            if (BadgeRows->state10 == 0xB) {
                SetMenuFrameOptions(0, 1, 0);
                DebugPrintStub(NewCar0Msg);
                savedCursor = LoadCursor;
                InitMenuArenas();
                InitCarPreviewScene();
                ModeGroup = r;
                ModeIndex = 0x22;
                SubmodeState = 0;
                LoadCursor = savedCursor;
                DebugPrintStub(StrNewCarLog);
            }
        }
    }
}
