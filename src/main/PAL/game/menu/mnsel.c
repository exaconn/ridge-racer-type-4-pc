#include "rr/menu_grid.h"
#include "rr/ui.h"
#include "rr/badge_row_types.h"
#include "rr/menu_state_types.h"
#include "game/menu/mnsel.h"
#include "rr/bios_devices.h"
#include "game/data/selection.h"
#include "rr/badge_rows.h"
#include "game/menu/mnrows.h"

extern void PlaySfx();
extern int MenuStepDirection;
extern MenuSettings2C SelectionState;
extern short D_800AD520;
extern int ModeFrame;
extern short D_800F3A26;
extern unsigned short D_800F3A42;
extern int D_800F3A60;
extern short ModeGroup;
extern short D_800F4D8A;
extern MenuGlyphRow1C *GlyphRows;
extern char *Dialogue;

void StepModeSelectChoice(void) {
    int s0;
    u8 *q;
    MenuSettings2C *r;
    int t = SelectionRows[0];
    D_800AD520 = t;
    ((u8 *)UiRows)[t + 0xC] = 0;
    do {
        if ((D_800F3A42 & 0x2000) && D_800AD520 < 3) {
            D_800AD520 = (unsigned short)D_800AD520 + 1;
            MenuStepDirection = 2;
        } else if ((D_800F3A42 & 0x8000) && D_800AD520 > 0) {
            D_800AD520 = (unsigned short)D_800AD520 - 1;
            MenuStepDirection = 1;
        } else {
            MenuStepDirection = 0;
            break;
        }
        PlaySfx(0);
        if (D_800AD520 < 0) {
            D_800AD520 = 0;
        } else if (D_800AD520 >= 4) {
            D_800AD520 = 3;
        }
        SelectionRows[0] = D_800AD520;
        Dialogue[4] = (unsigned char)D_800AD520 + 0x22;
        GlyphRows[D_800AD520].control02 = -6;
        GlyphRows[D_800AD520].flags00 = 0;
        UiRows->byte16 = D_800AD520;
    } while (0);
    q = UiRows->cells;
    q[D_800AD520] = 2;
    if (D_800F3A60 != 0) {
        s0 = BadgeRows->state10;
        if (s0 == 3) {
            PlaySfx(1);
            ModeFrame = 0;
            ResetScreenCounters();
            r = &SelectionState;
            r->mode1A = 1;
            r->copies[0].bytes[0] = SelectionRows[0];
            BadgeRows->state10 = 4;
            UiRows->byte14 = s0;
            ModeGroup = 1;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
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
    register s32 six = 6;
    register s32 quotient = six / count;
    register s32 offset;
    register MenuGridPointerPage *page;
    /* ASM_BARRIER: retain the captured record in V1 through the copy. */
    register MenuGrid38 *entry asm("$3");
    register MenuGrid38 *next;
    register MenuGrid38 *last;
    const GridRow8 *row, *table;
    register s32 step_y;
    s32 step_x;
    register s32 layout_offset;
    /* ASM_BARRIER: retain both captured packed words in T4/T5 until stores. */
    register u32 first asm("$12"), second asm("$13");
    /* PAL RAM page owns the full-width record-pointer slot at offset 0x3A28. */
    page = (MenuGridPointerPage *)0x800F0000U;
    offset = index * sizeof(MenuGrid38);
    step_y = y;
    /* ASM_BARRIER: capture the full stack argument before its signed16 narrowing. */
    __asm__("" : "+r"(step_y));
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
    table = D_8009FDA8;
    entry->step_y0A = step_y;
    next = (MenuGrid38 *)(offset + (unsigned long)page->records3A28);
    row = (const GridRow8 *)((u8 *)table + layout_offset);
    entry->step_x08 = step_x;
    first = row->first;
    second = row->second;
    /* ASM_BARRIER: stage the captured first word before the packed destination writes. */
    __asm__("" : : "r"(first));
    ((GridRow8 *)&entry->u30)->first = first;
    ((GridRow8 *)&entry->u30)->second = second;
    next->du32 = next->du32 / next->columns2E;
    last = (MenuGrid38 *)(offset + (unsigned long)page->records3A28);
    last->dv33 = last->dv33 / last->rows2F;
}
