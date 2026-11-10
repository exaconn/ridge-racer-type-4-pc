#include "game/menu/mnlist.h"
#include "rr/runtime_settings.h"
#include "game/system/pad.h"
#include "rr/sprite_emit.h"
#include "game/gfx/fontdraw.h"
#include "game/data/bankrec.h"
#include "game/ui/uitext.h"
#include "rr/ui_bar.h"
#include "rr/badge_row_types.h"
#include "rr/menu_state_types.h"
#include "rr/selection_types.h"
#include "rr/badge_rows.h"
#include "rr/menu_selection.h"
#include "rr/string_tail_copy.h"
/* RODATA 0x80010ecc 0x34 */




extern int FormatDecimalDigits();
extern UiUvRect UiFontGlyphs[];
extern char D_8009FDBF[];
extern char D_8009FDC0[];
extern char D_8009FDC4[];
extern u8 D_8009FDCC[];
extern u8 D_8009FDD8[];
extern u8 D_8009FDE4[];
extern SelectionRow SelectionRows[];
extern void *AddTileSemiTransPrim();
extern u8 D_8009FDF4[];
extern u8 D_8009FDF8[];
extern u8 D_8009FDFC[];
extern u8 D_8009FE00[];
extern u8 D_8009FE04[];
extern u8 D_8009FE08[];
extern u8 D_8009FE0C[];
extern MenuSettings2C SaveSlotSelections[];
extern short GpSaveSlotCursor;
extern char FractionText[];
extern int D_800F4CB8;


void *DrawCarSpecBadgePanel(u32 *arg0, void *arg1, s32 arg2)
{
  char sp28[8];
  int sp30;
  int sp34;
  int sp38;
  int sp3C;
  int sp40;
  int sp44;
  int sp48;
  char *sp4C;
  char *a1p;
  int x;
  int y;
  int tx;
  int i;
  int s1;
  int s2;
  int s6;
  int j2;
  int v;
  int t3;
  int t4;
  char *rec;
  u8 *e;
  signed char *f;
  SelectionRow *ent;
  CarRecord *record;
  char *q;
  char *q8;
  x = BadgeRows[arg2].x04;
  y = BadgeRows[arg2].y06;
  sp30 = BadgeRows[arg2].width0C;
  sp3C = (SelectionRows[ActivePlayerIndex].variant * 2) + 8;
  sp38 = SelectionRows[ActivePlayerIndex].selection * 2;
  sp34 = (SelectionRows[ActivePlayerIndex].parameter ^ 0x27) == 0;
  if ((SelectionRows[ActivePlayerIndex].parameter ^ 0x27) == 0)
  {
    if (SelectionRows[ActivePlayerIndex].selection == 2)
    {
      sp4C = D_8009FDC4;
    }
    else
    {
      if (SelectionRows[ActivePlayerIndex].selection == 0)
      {
        CopyStringClearTail(D_8009FDBF, "D ");
      }
      else
      {
        CopyStringClearTail(D_8009FDBF, "G ");
      }
      q = D_8009FDC0;
      FormatDecimalDigits(q, SelectionRows[ActivePlayerIndex].variant + 1, 0);
      sp4C = q - 8;
    }
  }
  record = CarRecordPtrs[ActivePlayerIndex];
  sp48 = 0x11;
  sp44 = record->stage1F3 + 1;
  sp40 = record->topSpeed1F0;
  if (record->handling1C8.steeringMode0A > 0)
  {
    sp48 = 0x12;
  }
  i = 7;
  q8 = &sp28[7];
  do
  {
    *q8 = 0;
    i--;
    q8--;
  }
  while (i >= 0);
  y += 0x1E;
  for (i = 0; i < 0xC; i++)
  {
    x += D_8009FDCC[i];
    e = D_8009FDD8 + i;
    arg1 = AddSpritePrim(arg0, arg1, x, y, &UiFontGlyphs[*e], 0x7FF8, 0);
    t3 = x + 2;
    s2 = t3 + UiFontGlyphs[*e].w;
    if (ImageRows->f1 != 6)
    {
      switch (i)
      {
        case 1:

        case 7:
          if (sp34 == 0)
        {
          if (i == 1)
          {
            s6 = sp3C;
          }
          else if (i == 7)
          {
            s6 = sp38;
          }
          for (s1 = s6; s1 < (s6 + 2); s1++)
          {
            f = ((signed char *) D_8009FDE4) + s1;
            arg1 = AddSpritePrim(arg0, arg1, s2, y, &UiFontGlyphs[*f], 0x7FF8, 0);
            t4 = s2 + 2;
            s2 = t4 + UiFontGlyphs[*f].w;
          }
        }
        else
        {
          arg1 = AddTilePrim(arg0, arg1, s2, y + 8, 0x38, 2, 0xFF, 0xFF, 0xFF);
        }
          break;

        case 3:
          if (sp34 == 0)
        {
          arg1 = AddTextSprites(arg0, arg1, s2, y, CarRecordPtrs[ActivePlayerIndex]->name1F5, 0x7FF8, 2, 0);
        }
        else
        {
          arg1 = AddTextSprites(arg0, arg1, s2, y, sp4C, 0x7FF8, 2, 0);
        }
          break;

        case 5:

        case 8:
          s6 = 2;
          if (i == 5)
        {
          s1 = sp44;
          s6 = 1;
        }
        else
        {
          s1 = sp40;
        }
          FormatDecimalDigits(sp28, s1, s6);
          arg1 = AddTextSprites(arg0, arg1, s2, y, sp28, 0x7FF8, 0, 0);
          break;

        case 11:
          arg1 = AddSpritePrim(arg0, arg1, s2, y, &UiFontGlyphs[sp48], 0x7FF8, 0);
          break;

      }

    }
    if (i == 5)
    {
      y += 0x1C;
      x = BadgeRows[arg2].x04;
    }
  }

  arg1 = AddDrawModePrim(arg0, arg1, 0x38);
  y = y - 8;
  x = BadgeRows[arg2].x04;
  arg1 = AddTilePrim(arg0, arg1, x, y, sp30, 2, 0xFF, 0xFF, 0xFF);
  for (i = 0; i < 4; i++)
  {
    v = -0x18;
    if (!(i < 2))
    {
      v = 4;
    }
    sp30 = 0xE4;
    if (i & 1)
    {
      sp30 = 0x78;
    }
    arg1 = AddTilePrim(arg0, arg1, x + sp30, y + v, 1, 0x14, 0xFF, 0xFF, 0xFF);
  }

  return arg1;
}

void *DrawGpSaveSlotsPanel(u32 *arg0, void *arg1, s32 arg2)
{
    int sp28[3];
    char sp38[8];
    int sp40;
    int x;
    int y;
    int i;
    int j;
    char *p;

    x = BadgeRows[arg2].x04;
    y = BadgeRows[arg2].y06;
    sp40 = BadgeRows[arg2].width0C;
    y += 0x20;

    for (i = 0; i < 4; i++) {
        x += D_8009FDF4[i];
        arg1 = AddSpritePrim(arg0, arg1, x, y, &UiFontGlyphs[D_8009FDFC[i]], 0x7FF8, 0);
    }

    i = 7;
    p = &sp38[7];
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);

    y -= 8;
    for (i = 0; i < 3; i++) {
        x = BadgeRows[arg2].x04 + 8;
        FormatDecimalDigits(sp38, i + 1, 0);
        arg1 = AddTextSprites(arg0, arg1, x, y + D_8009FDF8[i + 1], sp38, 0x7FF8, 0, 0);
        if ((sp28[2] = SaveSlotSelections[i].phase17) != -1) {
            sp28[0] = D_8009FE00[SaveSlotSelections[i].copies[0].bytes[0] & 3];
            sp28[1] = D_8009FE04[SaveSlotSelections[i].copies[0].bytes[1] & 3];
            sp28[2] = D_8009FE08[SaveSlotSelections[i].phase17 / 2];
            for (j = 0; j < 3; j++) {
                x += D_8009FDF4[j + 1];
                arg1 = AddSpritePrim(arg0, arg1, x, y + D_8009FDF8[i + 1], &UiFontGlyphs[sp28[j]], 0x7FF8, 0);
                if (j == 2 && SaveSlotSelections[i].phase17 < 4) {
                    x += 0xE;
                    arg1 = AddSpritePrim(arg0, arg1, x, y + D_8009FDF8[i + 1], &UiFontGlyphs[38], 0x7FF8, 0);
                }
            }
        } else {
            for (j = 0; j < 3; j++) {
                x += D_8009FDF4[j + 1];
                arg1 = AddTilePrim(arg0, arg1, x, y + D_8009FDF8[i + 1] + 8, D_8009FE0C[j], 2, 0xFF, 0xFF, 0xFF);
            }
        }
        if (i == GpSaveSlotCursor) {
            arg1 = AddTileSemiTransPrim(arg0, arg1, BadgeRows[arg2].x04, y + D_8009FDF8[i + 1] - 4, sp40, 0x18, 0x20, 0x20, 0x20);
        }
    }

    arg1 = AddDrawModePrim(arg0, arg1, 0x38);
    y += 0x1C;
    x = BadgeRows[arg2].x04;
    arg1 = AddTilePrim(arg0, arg1, x, y, sp40, 2, 0xFF, 0xFF, 0xFF);
    y -= 0x1C;
    for (i = 0; i < 4; i++) {
        x = BadgeRows[arg2].x04;
        for (j = 0; j < 3; j++) {
            x += D_8009FDF4[j + 1];
            arg1 = AddTilePrim(arg0, arg1, x, y + D_8009FDF8[i], 2, 0x10, 0xFF, 0xFF, 0xFF);
        }
    }

    return arg1;
}


char *FormatCountOfTotal(int a, int b, int c) {
    int i = 7;
    char *p = FractionText + i;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    FormatDecimalDigits(FractionText, a, c);
    FractionText[c + 1] = 0x2F;
    FormatDecimalDigits(FractionText + c + 2, b, c);
    return FractionText;
}


void *DrawControllerConfigPanel(u32 *ordering, void *cursor, s32 row)
{
  char text[8];
  int character;
  char *textCursor;
  int x;
  int y;
  int textX;
  int layout;
  int controller;
  int protocol;
  void *result;
  x = BadgeRows[row].x04 + 8;
  y = BadgeRows[row].y06 + 0x24;
  character = 7;
  textCursor = text + 7;
  do
  {
    *textCursor = 0;
    character--;
    textCursor--;
  }
  while (character >= 0);
  controller = (D_800F4CB8 - 1U) & 1;
  if (controller < 0)
  {
    controller = 0;
  }
  protocol = PlayerControllers[controller].protocol10;
  switch (protocol)
  {
    case 0:

    default:
      layout = Options.config.f0[controller].buttonLayout00;
      break;

    case 1:
      layout = Options.config.p[controller].buttonLayout0C;
      break;

    case 2:
      layout = Options.config.w[controller].buttonLayout03;
      break;

  }

  result = AddSpritePrim(ordering, cursor, x, y, &MenuStepArrowUv, 0x7FF8, 0);
  text[0] = layout + 0x41;
  textX = x + 2;
  x = textX + MenuStepArrowUv.w;
  result = AddDrawModePrim(ordering, AddTextSprites(ordering, result, x, y, text, 0x7FF8, 0, 0), 0x38);
  return result;
}
