#include "game/ui/uidraw.h"
#include "game/sound/sndvab.h"
#include "rr/save_slots.h"
#include "rr/menu_text_state.h"
#include "rr/menu_grid.h"
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "rr/menu_state_types.h"
#include "game/menu/mnsel.h"
#include "rr/bios_devices.h"
#include "game/data/selection.h"
#include "rr/badge_rows.h"
#include "game/menu/mnrows.h"

extern int MenuStepDirection;
extern short SelectedTeamIndex;
extern int ModeFrame;
extern short ModeIndex;
extern unsigned short MenuSelectedCarSlot;
extern int MenuReturnSubmodeIndex;
extern short ModeGroup;
extern short SubmodeState;

void StepModeSelectChoice(void) {
    int s0;
    u8 *q;
    MenuSettings2C *r;
    int t = SelectionRows.bytes[0];
    SelectedTeamIndex = t;
    ((u8 *)UiRows)[t + 0xC] = 0;
    if ((MenuSelectedCarSlot & 0x2000) && SelectedTeamIndex < 3) {
        SelectedTeamIndex = (unsigned short)SelectedTeamIndex + 1;
        MenuStepDirection = 2;
    } else if ((MenuSelectedCarSlot & 0x8000) && SelectedTeamIndex > 0) {
        SelectedTeamIndex = (unsigned short)SelectedTeamIndex - 1;
        MenuStepDirection = 1;
    } else {
        MenuStepDirection = 0;
        goto skip_step;
    }
    PlaySfx(0);
    if (SelectedTeamIndex < 0) {
        SelectedTeamIndex = 0;
    } else if (SelectedTeamIndex >= 4) {
        SelectedTeamIndex = 3;
    }
    SelectionRows.bytes[0] = SelectedTeamIndex;
    Dialogue->b4 = (unsigned char)SelectedTeamIndex + 0x22;
    GlyphRows[SelectedTeamIndex].control02 = -6;
    GlyphRows[SelectedTeamIndex].frameIndex00 = 0;
    UiRows->byte16 = SelectedTeamIndex;
skip_step:;
    q = UiRows->cells;
    q[SelectedTeamIndex] = 2;
    if (MenuReturnSubmodeIndex != 0) {
        s0 = BadgeRows->state10;
        if (s0 == 3) {
            PlaySfx(1);
            ModeFrame = 0;
            ResetScreenCounters();
            r = &SelectionState.fields;
            r->mode1A = 1;
            r->copies[0].bytes[0] = SelectionRows.bytes[0];
            BadgeRows->state10 = 4;
            UiRows->byte14 = s0;
            ModeGroup = 1;
            ModeIndex = 0xB;
            SubmodeState = 0;
        }
    }
}

int AllocMenuGridRecords(void *buffer, int count) {
    int sz = count * sizeof(*MenuGridRecords);
    MenuGridRecords = buffer;
    PsyqBzero(buffer, sz);
    return sz;
}


void InitMenuGrid(s32 index, s32 layout, s32 count, s32 x, s32 y) {
    s32 six = 6;
    s32 quotient = six / count;
    s32 offset;
    MenuGridPointerPage *page;
    MenuGrid38 *entry;
    MenuGrid38 *next;
    MenuGrid38 *last;
    const GridRow8 *row, *table;
    s32 step_y;
    s32 step_x;
    s32 layout_offset;
    /* PAL RAM page owns the full-width record-pointer slot at offset 0x3A28. */
    page = (MenuGridPointerPage *)0x800F0000U;
    offset = index * sizeof(MenuGrid38);
    step_y = y;
    entry = (MenuGrid38 *)(offset + (unsigned long)page->records3A28);
    entry->columns2E = quotient * 3;
    ((MenuGrid38 *)(offset + (unsigned long)page->records3A28))->rows2F = six;
    entry = (MenuGrid38 *)(offset + (unsigned long)page->records3A28);
    step_y = (s16)step_y / entry->rows2F;
    step_x = (s16)x / (s32)((u32)count * entry->columns2E);
    layout_offset = layout * 8;
    entry->count2C = count;
    entry->width04 = 64;
    entry->height06 = 8;
    table = SelectionAnimStepTable;
    entry->step_y0A = step_y;
    next = (MenuGrid38 *)(offset + (unsigned long)page->records3A28);
    row = (const GridRow8 *)((u8 *)table + layout_offset);
    entry->step_x08 = step_x;
    *(GridRow8 *)&entry->u30 = *row;
    next->du32 = next->du32 / next->columns2E;
    last = (MenuGrid38 *)(offset + (unsigned long)page->records3A28);
    last->dv33 = last->dv33 / last->rows2F;
}
