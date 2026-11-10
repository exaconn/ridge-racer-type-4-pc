#include "types.h"
#include "game/ui/barsel0.h"
#include "rr/badge_rows.h"
#include "game/menu/menu13.h"
#include "game/ui/uidraw.h"
#include "game/ui/uiwin.h"

extern int SetMenuFrameOptions();
extern int DrawMenuStepArrows();
extern int UpdateDialogue();
extern s16 D_800F3826;
extern u8 D_800F4D99;
extern void *Dialogue;

s32 UpdateResultsBarSelection(void) {
    s32 var_s2;

    var_s2 = 0;
    if (((s16 *)BadgeRows)[8] == 3) {
        SkipDialoguePage(Dialogue);
        UpdateDialogue(((s16 *)BadgeRows)[4] + 0xA, ((s16 *)BadgeRows)[5] + 0x28, Dialogue);
    }
    UpdateBadgeRowAnim(0);
    if (((s8 *)Dialogue)[4] >= 0x22) {
        if (((s16 *)BadgeRows)[8] == 0xB && D_800AD4F2 == 1) {
            ((s16 *)BadgeRows)[8] = 0;
        } else if (((s16 *)BadgeRows)[8] < 4) {
            DrawMenuStepArrows(0xA0, 0xCC, 0x30, 0, D_800F3826, 0, 4);
            var_s2 = UpdateGlyphRow(D_800F4D99);
        }
    }
    SetMenuFrameOptions(1, 1, 0);
    return var_s2;
}
