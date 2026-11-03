#include "types.h"
#include "rr/save_progress.h"
#include "rr/menu_choice.h"
#include "rr/course_choice_types.h"

extern s32 func_8002F8E4(void);
extern void func_80037040(s8 *, s32);
extern s32 func_8003708C(void *);

extern RrCourseChoice4 D_800ABE08[2][9];
extern u8 D_800AD4F4[];
extern u8 D_800AD4F8[][4];
extern u16 D_800F4BE0;

void func_8002F958(void) {
    u8 sp10[4];
    s32 old_count;
    s32 i, k;
    RrMenuSettings2C *settings;
    s32 glyph;
    s32 j;
    s32 found;
    s32 slot;
    s32 new_mask;
    s32 g;

    old_count = func_8002F8E4();
    i = 0;

#define MARK_PROGRESS(c) D_800F3920.prefix.progress.progress10[(c).bytes[0]][(c).bytes[1]] |= 1 << (c).bytes[2]
    MARK_PROGRESS(D_800AD4D8.copies[0]);
    D_800F3920.history6C[3] = *(RrSelectionRow *)&D_800AD4D8.copies[0];
    do {
        settings = (RrMenuSettings2C *)((u8 *)&D_800AD4D8 + i * 4);
        MARK_PROGRESS(settings->copies[1]);
        D_800F3920.history6C[i] = *(RrSelectionRow *)&settings->copies[1];
        i++;
    } while (i < 3);

#undef MARK_PROGRESS

    if (old_count < func_8002F8E4()) {
        D_800F4BE0 |= 1;
    }

    slot = 0;
    for (k = 0; k < 8; k++) {
        g = D_800ABE08[1][k].glyph;
        func_80037040((s8 *)&D_800F4D98, g);
        if (D_800F4D98.bytes[2] == 0x27 && slot < 4) {
            do {
                if (slot == 3) {
                    glyph = func_8003708C(D_800AD4F4);
                } else {
                    glyph = func_8003708C(D_800AD4F8[slot]);
                }
                found = 0;
                for (j = 0; j < 8; j++) {
                    func_80037040((s8 *)sp10, D_800ABE08[1][j].glyph);
                    sp10[2] %= 20;
                    if (glyph == func_8003708C(sp10)) {
                        found = 1;
                        break;
                    }
                }

                if (found == 0) {
                    D_800ABE08[1][k].glyph = glyph;
                    D_800ABE08[1][k].second = 0;
                    D_800ABE08[1][k].first = 0;
                    break;
                }
                slot++;
            } while (slot < 4);
        }
    }

    i = D_800AD4D8.reserved10[3];
    if (i < 99) {
        i++;
    }
    D_800F3920.unlocked60[D_800AD4D8.reserved10[2]] = i;

    D_800F3920.prefix.progress.prefix00.fields.configuration0C |= 0x100;
    old_count = D_800F3920.prefix.reserved50.words[1];
    D_800F3920.prefix.reserved50.words[1] = old_count | 1;
    if (old_count != (old_count | 1)) {
        D_800F4BE0 |= 2;
    }

    for (i = 4, slot = 0; i < 8; i++) {
        slot += D_800AD4D8.retries08[i];
    }

    for (i = 0, old_count = 0; i < 4; i++) {
        old_count |= D_800F3920.prefix.reserved50.bytes[i + 8] << i;
    }

    if (D_800AD4D8.copies[0].bytes[2] == 0x12 && slot == 12) {
        D_800F3920.prefix.reserved50.bytes[D_800AD4D8.copies[0].bytes[0] + 8] = 1;
    }

    for (i = 0, new_mask = 0; i < 4; i++) {
        new_mask |= D_800F3920.prefix.reserved50.bytes[i + 8] << i;
    }

    if (new_mask != old_count) {
        D_800F4BE0 |= 4;
    }
}
