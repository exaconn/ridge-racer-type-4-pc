#include "rr/save_slots.h"
#include "rr/menu_choice_types.h"
#include "types.h"
#include "rr/save_progress.h"
#include "rr/menu_choice.h"
#include "rr/course_choice_types.h"
#include "game/menu/menutran.h"
#include "game/menu/menusave.h"
#include "rr/menu_glyph.h"
#include "game/menu/mnchoice.h"


extern CourseChoice4 CourseChoiceTable[4][9];

void UpdateSaveProgress(void) {
    u8 sp10[4];
    s32 old_count;
    s32 i, k;
    s32 glyph;
    s32 j;
    s32 found;
    s32 slot;
    s32 new_mask;
    s32 g;

    old_count = CountProgressFlags();
    i = 0;

#define MARK_PROGRESS(c) SaveProgress.prefix.progress.progress10[(c).bytes[0]][(c).bytes[1]] |= 1 << (c).bytes[2]
    MARK_PROGRESS(SelectionState.fields.copies[0]);
    SaveProgress.history6C[3] = *(SelectionRow *)&SelectionState.fields.copies[0];
    do {
        MARK_PROGRESS(SelectionState.fields.copies[1 + i]);
        SaveProgress.history6C[i] = *(SelectionRow *)&SelectionState.fields.copies[1 + i];
        i++;
    } while (i < 3);

#undef MARK_PROGRESS

    if (old_count < CountProgressFlags()) {
        UnlockNoticeMask |= 1;
    }

    slot = 0;
    for (k = 0; k < 8; k++) {
        g = CourseChoiceTable[1][k].glyph;
        UnpackChoiceGlyph((s8 *)&SelectionRows, g);
        if (SelectionRows.bytes[2] == 0x27 && slot < 4) {
            do {
                if (slot == 3) {
                    glyph = PackChoiceGlyph(SelectionState.fields.copies[0].bytes);
                } else {
                    glyph = PackChoiceGlyph(SelectionState.fields.copies[1 + (slot)].bytes);
                }
                found = 0;
                for (j = 0; j < 8; j++) {
                    UnpackChoiceGlyph((s8 *)sp10, CourseChoiceTable[1][j].glyph);
                    sp10[2] %= 20;
                    if (glyph == PackChoiceGlyph(sp10)) {
                        found = 1;
                        break;
                    }
                }

                if (found == 0) {
                    CourseChoiceTable[1][k].glyph = glyph;
                    CourseChoiceTable[1][k].second = 0;
                    CourseChoiceTable[1][k].first = 0;
                    break;
                }
                slot++;
            } while (slot < 4);
        }
    }

    i = SelectionState.fields.reserved10[3];
    if (i < 99) {
        i++;
    }
    SaveProgress.unlocked60[SelectionState.fields.reserved10[2]] = i;

    SaveProgress.prefix.progress.prefix00.fields.configuration0C |= 0x100;
    old_count = SaveProgress.prefix.unlocks50.mainMenuUnlocks54;
    SaveProgress.prefix.unlocks50.mainMenuUnlocks54 = old_count | 1;
    if (old_count != (old_count | 1)) {
        UnlockNoticeMask |= 2;
    }

    for (i = 4, slot = 0; i < 8; i++) {
        slot += SelectionState.fields.retries08[i];
    }

    for (i = 0, old_count = 0; i < 4; i++) {
        old_count |= SaveProgress.prefix.unlocks50.bonusUnlocks58[i] << i;
    }

    if (SelectionState.fields.copies[0].bytes[2] == 0x12 && slot == 12) {
        SaveProgress.prefix.unlocks50.bonusUnlocks58[SelectionState.fields.copies[0].bytes[0]] = 1;
    }

    for (i = 0, new_mask = 0; i < 4; i++) {
        new_mask |= SaveProgress.prefix.unlocks50.bonusUnlocks58[i] << i;
    }

    if (new_mask != old_count) {
        UnlockNoticeMask |= 4;
    }
}

/* Initial values of the globals this module owns. */
SaveProgressRecord7C SaveProgress;
short UnlockNoticeMask;
