#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "game/menu/menu8.h"
#include "game/menu/menuslot.h"
#include "rr/menu_background.h"
#include "rr/menu_text_state.h"
#include "rr/bios_devices.h"
#include "game/data/ldscn1.h"
#include "game/data/screenframe.h"
#include "game/data/menuvars.h"
#include "rr/course_choices.h"
#include "rr/screen_tiles.h"
#include "game/ui/badge1.h"
#include "rr/menu_label_render.h"
#include "rr/trig.h"
#include "game/ui/uidraw.h"
extern int StepMenuValue();
extern int DrawUiRow();
extern void PlaySfx();
extern short D_800AC5D0;
extern short MenuCursor;
extern short D_800F3A26;
extern unsigned short D_800F4D8A;
extern int ModeFrame;
extern int D_800F3A60;
extern int D_800F4D74;
extern BadgeRow1C *BadgeRows;
extern struct { s32 unk00; char b4; char unk05[2]; char b7; } *Dialogue;
extern int D_800F4CA8;
extern char D_8009F648[];

void YesNoPromptModeUpdate(void);

void YesNoPromptModeUpdate(void) {
    int r;
    ModeFrame += 1;
    switch (D_800F4D8A) {
    case 0:
        UiRows[MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, &MenuCursor, 2, 0);
        UiRows[MenuCursor].cells[0] = 2;
        if (D_800F3A60 != 0) {
            if (UiRows->byte14 == 2) {
                PlaySfx(1);
                UiRows->byte14 = 3;
                UiRows[1].byte14 = 3;
                BadgeRows->state10 = 4;
                UiRows[MenuCursor].cells[0] = 1;
                D_800AC5D0 = 0x40;
                LoadCursor = D_800F4D74;
                ResetScreenCounters();
                if (MenuCursor == 0) {
                    D_800F4D8A = 3;
                } else if (MenuCursor == 1) {
                    D_800F4D8A = 4;
                }
            }
        }
        break;
    case 3:
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            r = LoadMenuCarModelsStep();
            if (r == 1) {
                ModeGroup = r;
                D_800F3A26 = 0x47;
                D_800F4D8A = 0;
            }
        }
        break;
    case 4:
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            r = LoadTimListStep(1);
            if (r == 1) {
                ModeGroup = r;
                D_800F3A26 = 0xF;
                D_800F4D8A = 0;
            }
        }
        break;
    }
    if (BadgeRows->state10 == 3) {
        Dialogue->b7 = 0;
        Dialogue->b4 = 0x26;
        UpdateDialogue(BadgeRows->x08 + 0xA, BadgeRows->y0A + 0x28, Dialogue);
    }
    UpdateBadgeRowAnim(0);
    DrawUiRow(0);
    DrawUiRow(1);
    UpdateScreenFade(0x10084);
}

void InitDialogueState(signed char *a0) {
    PsyqBzero(a0, 0x348);
    a0[5] = -1;
    a0[4] = 0;
    if (D_800F4CA8 == 1) {
        SelectDialogueScript();
    }
}

void DrawDialogueContinueMark(int a0, int a1) {
  int v = rsin((ScreenFrame << 6) & 0xFC0);
  void *new_var;
  if (v < 0)
  {
    v += 0x7F;
  }
  new_var = *((void **) SCRATCHPAD);
  *((void **) SCRATCHPAD) = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], new_var, a0, a1, D_8009F648, 0x1B56, ((v >> 7) + 0x80) * 2), 0x25);
}
