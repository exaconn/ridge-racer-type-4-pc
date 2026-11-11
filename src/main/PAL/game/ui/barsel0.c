#include "game/ui/uipart1.h"
#include "rr/menu_text_state.h"
#include "types.h"
#include "game/ui/barsel0.h"
#include "rr/badge_rows.h"
#include "game/menu/menu13.h"
#include "game/ui/uidraw.h"
#include "game/ui/uiwin.h"

extern s16 SelectedMakerIndex;
#include "game/data/selection.h"

/* The results controller supplies its direction word; this routine currently
 * ignores it. Keep that observed call slot in the shared interface. */
s32 UpdateResultsBarSelection(s32 direction) {
    s32 var_s2;

    var_s2 = 0;
    if (((s16 *)BadgeRows)[8] == 3) {
        SkipDialoguePage(Dialogue);
        UpdateDialogue(((s16 *)BadgeRows)[4] + 0xA, ((s16 *)BadgeRows)[5] + 0x28, Dialogue);
    }
    UpdateBadgeRowAnim(0);
    if (Dialogue->b4 >= 0x22) {
        if (((s16 *)BadgeRows)[8] == 0xB && MenuActiveDialogId == 1) {
            ((s16 *)BadgeRows)[8] = 0;
        } else if (((s16 *)BadgeRows)[8] < 4) {
            DrawMenuStepArrows(0xA0, 0xCC, 0x30, 0, SelectedMakerIndex, 0, 4);
            var_s2 = UpdateGlyphRow(SelectionRows.choice.bytes[1]);
        }
    }
    SetMenuFrameOptions(1, 1, 0);
    return var_s2;
}
