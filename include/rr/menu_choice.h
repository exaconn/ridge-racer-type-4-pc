#ifndef RR_MENU_CHOICE_H
#define RR_MENU_CHOICE_H

#include "rr/ui.h"
#include "rr/load_cursor.h"
#include "rr/badge_rows.h"

/* Byte-aligned records deliberately use the original unaligned word transfers. */
#include "rr/menu_state_types.h"
#include "game/menu/menuwork.h"

extern s32 MenuStepDirection, D_800F3A60;
extern void  *MenuArenaBase;
extern MenuSettings2C SelectionState;
extern s16 D_800F3826;
extern u16 D_800F3A42;
extern MenuGlyphRow1C *GlyphRows;
extern MenuChoice4 SelectionRows;
extern u8 D_800F4D99; /* Alias of SelectionRows.bytes[1]. */
extern u8 *Dialogue;
void PlaySfx(s32 mode);
void ResetScreenCounters(void);
void SetMenuFrameOptions(s32 first, s32 second, s32 third);

/* Current UI backing must cover byte12+the initial byte selection (up to267).
 * Movement requires four glyph rows and byte4 of the highlight buffer. Helpers
 * may change selection, backing pointers and records. Acceptance uses captured
 * state3 but fresh row/UI pointers after the sound helper. Publishes the fresh
 * full buffer pointer after both state helpers. */
void StepResultsMenuChoice(void);
RR_ASSERT_SIZE(MenuChoice4, 4);
RR_ASSERT_SIZE(MenuGlyphRow1C, 0x1C);
RR_ASSERT_OFFSET(MenuGlyphRow1C, control02, 2);
RR_ASSERT_OFFSET(MenuGlyphRow1C, glyph07, 7);
RR_ASSERT_OFFSET(MenuGlyphRow1C, x08, 8);
RR_ASSERT_OFFSET(MenuGlyphRow1C, phase18, 0x18);

#endif
