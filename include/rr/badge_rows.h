#ifndef RR_BADGE_ROWS_H
#define RR_BADGE_ROWS_H

#include "rr/ui_bar.h"
#include "rr/icon_sprites.h"

typedef struct {
    u16 status00;
    s16 half02;
    s16 x04, y06, x08, y0A, width0C;
    s16 extent0E;
    s16 state10;
    u8 pad12[2];
    s16 x14, y16, w18, h1A;
} RrBadgeRow1C;

extern RrBadgeRow1C *D_800F3AFC;
extern s8 D_8009F108[];
extern RrUiUvRect D_8009F074[];

/* The helpers thread full packet pointers. Coordinate arguments remain wide
 * until their wire16 stores, including sums of two signed16 row fields. */
void *func_8001FD20(u32 *, void *, s32, s32, s32, s32, s32, s32, s32);
void *func_8001FDD4(u32 *, void *, s32, s32, s32, s32, s32, s32, s32);
void *func_80020074(u32 *, void *, s32, s32, s32, s32, s32, s32,
                    u16, u16, u16, u16);
void *func_8001FC28(u32 *, void *, s32, s32, s32, s32);

/* Emits five row underlines/areas and four textured badges. Style selects an
 * eight-byte pattern group, with nine readable bytes; its first four signed
 * glyph indices must name accessible UV rows. Index selects five consecutive
 * records and the byte offsets must fit signed32. Output helpers determine each
 * next cursor. Captures glyph/shift before the first helper, then reloads the
 * row/context pointers after every helper; UV bytes are read after the first.
 * Each helper input cursor and returned cursor must point into writable arenas. */
void *func_80030BEC(void *cursor, s32 style, s32 index);

RR_ASSERT_SIZE(RrBadgeRow1C, 0x1C);
RR_ASSERT_OFFSET(RrBadgeRow1C, status00, 0);
RR_ASSERT_OFFSET(RrBadgeRow1C, half02, 2);
RR_ASSERT_OFFSET(RrBadgeRow1C, x04, 4);
RR_ASSERT_OFFSET(RrBadgeRow1C, y06, 6);
RR_ASSERT_OFFSET(RrBadgeRow1C, width0C, 0xC);
RR_ASSERT_OFFSET(RrBadgeRow1C, x08, 8);
RR_ASSERT_OFFSET(RrBadgeRow1C, y0A, 0xA);
RR_ASSERT_OFFSET(RrBadgeRow1C, extent0E, 0xE);
RR_ASSERT_OFFSET(RrBadgeRow1C, state10, 0x10);
RR_ASSERT_OFFSET(RrBadgeRow1C, x14, 0x14);
RR_ASSERT_OFFSET(RrBadgeRow1C, y16, 0x16);
RR_ASSERT_OFFSET(RrBadgeRow1C, w18, 0x18);
RR_ASSERT_OFFSET(RrBadgeRow1C, h1A, 0x1A);

#endif
