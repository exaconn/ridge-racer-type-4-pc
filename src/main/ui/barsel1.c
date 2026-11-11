#include "game/ui/uipart1.h"
#include "rr/menu_text_state.h"
#include "types.h"
#include "game/ui/barsel1.h"
#include "rr/badge_rows.h"
#include "game/ui/barsel0.h"
#include "game/data/selection.h"
#include "game/menu/menu13.h"
#include "game/ui/uidraw.h"
#include "game/ui/uiwin.h"

extern s16 SelectedTeamIndex;

s32 UpdateModeSelectBarAnim(s32 direction) {
    s32 var_s2;

    var_s2 = 0;
    if (BadgeRows->state10 == 3) {
        SkipDialoguePage(Dialogue);
        UpdateDialogue(BadgeRows->x08 + 0xA, BadgeRows->y0A + 0x28, Dialogue);
    }
    UpdateBadgeRowAnim(0);
    if (Dialogue->b4 >= 0x22) {
        if (BadgeRows->state10 == 0xB && MenuActiveDialogId == 0) {
            BadgeRows->state10 = 0;
        } else if (BadgeRows->state10 < 4) {
            DrawMenuStepArrows(0xA0, 0xCC, 0x30, 0, SelectedTeamIndex, 0, 4);
            var_s2 = UpdateGlyphRow(SelectionRows.bytes[0]);
        }
    }
    SetMenuFrameOptions(1, 1, 0);
    return var_s2;
}
