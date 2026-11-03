#ifndef RR_MENU_CHOICE_H
#define RR_MENU_CHOICE_H

#include "rr/ui.h"
#include "rr/badge_rows.h"

/* Byte-aligned records deliberately use the original unaligned word transfers. */
#include "rr/menu_state_types.h"
#include "game/menu/menuwork.h"

extern s32 D_8009FDA4, D_800F3A60;
extern void *D_800ACC18, *D_800F4D40;
extern RrMenuSettings2C D_800AD4D8;
extern s16 D_800F3826;
extern u16 D_800F3A42;
extern RrMenuGlyphRow1C *D_800F4D8C;
extern RrMenuChoice4 D_800F4D98;
extern u8 D_800F4D99; /* Alias of D_800F4D98.bytes[1]. */
extern u8 *D_800FF68C;
void func_8004F24C(s32 mode);
void func_8003A198(void);
void func_8002AF8C(s32 first, s32 second, s32 third);

/* Current UI backing must cover byte12+the initial byte selection (up to267).
 * Movement requires four glyph rows and byte4 of the highlight buffer. Helpers
 * may change selection, backing pointers and records. Acceptance uses captured
 * state3 but fresh row/UI pointers after the sound helper. Publishes the fresh
 * full buffer pointer after both state helpers. */
void func_80036D40(void);
RR_ASSERT_SIZE(RrMenuChoice4, 4);
RR_ASSERT_SIZE(RrMenuSettings2C, 0x2C);
RR_ASSERT_OFFSET(RrMenuSettings2C, retries08, 8);
RR_ASSERT_OFFSET(RrMenuSettings2C, phase17, 0x17);
RR_ASSERT_OFFSET(RrMenuSettings2C, mode1A, 0x1A);
RR_ASSERT_OFFSET(RrMenuSettings2C, copies, 0x1C);
RR_ASSERT_SIZE(RrMenuGlyphRow1C, 0x1C);
RR_ASSERT_OFFSET(RrMenuGlyphRow1C, control02, 2);
RR_ASSERT_OFFSET(RrMenuGlyphRow1C, glyph07, 7);
RR_ASSERT_OFFSET(RrMenuGlyphRow1C, x08, 8);
RR_ASSERT_OFFSET(RrMenuGlyphRow1C, phase18, 0x18);

#endif
