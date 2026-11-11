#include "types.h"
#include "rr/frame_storage_types.h"
#include "scratchpad.h"
#include "rr/ui.h"
#include "game/race/ranking.h"
#include "game/menu/menutra22.h"
#include "game/ui/uitext.h"
#include "rr/badge_rows.h"

extern short CardMenuOperation;
extern char StrName[];
extern char TeamLabel[];
extern char StrMaker[];

void UpdateCardSlotUiRows(int a0, int a1);
void CardSlotUiNoop(void);
void DrawCardMenuCommandRows(int a0, int a1);
void MainLoopStartupHook(void);

void UpdateCardSlotUiRows(int a0, int a1) {
    int i = 0;
    UiRow24 *p, *q;
    int v;
    do {
        if (a1 == 1) {
            p = &((UiRow24 *)(i * 0x24 + (int)UiRows))[0x26];
            if (p->byte14 == 5) { p->byte14 = 0; goto tail; }
        }
        if (a1 == 0) {
            q = &((UiRow24 *)(i * 0x24 + (int)UiRows))[0x26];
            v = q->byte14;
            if (v == 2) {
                q->byte14 = 3;
            } else if (v == 0) {
                return;
            }
        }
tail:
        DrawUiRow(i + 0x26);
        if (i == a0) ((UiRow24 *)(i * 0x24 + (int)UiRows))[0x26].cells[0] = 2;
        else ((UiRow24 *)(i * 0x24 + (int)UiRows))[0x26].cells[0] = 0;
        i++;
    } while (i < 2);
}

void CardSlotUiNoop(void) {
}

void DrawCardMenuCommandRows(int a0, int a1) {
 int n, i;
 if (CardMenuOperation == 0) n = 4; else if (CardMenuOperation == 6) n = 3; else n = 2;
 i = 0;
 if (n != 0) {
  do {
   DrawUiRow(i + 0x22);
   if (a1 == 1) {
    if (i == a0) ((UiRow24 *)(i*0x24 + (int)UiRows))[0x22].cells[0] = 2;
    else ((UiRow24 *)(i*0x24 + (int)UiRows))[0x22].cells[0] = 0;
   } else {
    UiRow24 *p = &((UiRow24 *)(i*0x24 + (int)UiRows))[0x22];
    if ((s8)p->cells[0] == 2) p->cells[0] = 1;
   }
   i++;
  } while (i < n);
 }
}

void DrawSaveSlotTableHeader(int a0)
{
  int new_var;
  int new_var2;
  DrawSmallFontString(0x28, 0xA0, StrName, 0x414);
  DrawSmallFontString(0x68, 0xA0, TeamLabel, 0x414);
  DrawSmallFontString(0xC2, 0xA0, StrMaker, 0x414);
  new_var2 = *((int *) SCRATCHPAD);
  *((int *) SCRATCHPAD) = (new_var = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[1], new_var2, 0x20, (a0 << 5) + 0xB2, 0x104, 0xC, 0x80, 0x20, 0x20));
}

void MainLoopStartupHook(void) {

}
