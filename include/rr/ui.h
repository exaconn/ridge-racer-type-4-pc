#ifndef RR_UI_H
#define RR_UI_H

#include "rr/layout.h"

/* Shared 0x24-byte UI row. Cell clearing may extend into later fields/rows;
 * callers provide the complete writable backing allocation for count bytes. */
typedef struct {
    u8 reserved00, byte01, byte02, reserved03;
    s16 half04, half06, half08, half0A;
    u8 cells[8], byte14, byte15, byte16, byte17, pad18[8];
    s16 half20;
    u8 pad22[2];
} RrUiRow24;

extern s32 D_800F4BEC;
extern RrUiRow24 *D_800F4D80;
/* Legacy callers may append ignored words after the ten row arguments.
 * The signed upper-bound check accepts negative indices; these require a
 * readable/writable prefix. Positive count clears bytes beginning at cells. */
void func_8003E928(s32 index, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                   s32 count, s32 g, s32 h,...);

RR_ASSERT_SIZE(RrUiRow24, 0x24);
RR_ASSERT_OFFSET(RrUiRow24, byte01, 1);
RR_ASSERT_OFFSET(RrUiRow24, byte02, 2);
RR_ASSERT_OFFSET(RrUiRow24, half04, 4);
RR_ASSERT_OFFSET(RrUiRow24, half06, 6);
RR_ASSERT_OFFSET(RrUiRow24, half08, 8);
RR_ASSERT_OFFSET(RrUiRow24, half0A, 0xA);
RR_ASSERT_OFFSET(RrUiRow24, cells, 0xC);
RR_ASSERT_OFFSET(RrUiRow24, byte14, 0x14);
RR_ASSERT_OFFSET(RrUiRow24, byte15, 0x15);
RR_ASSERT_OFFSET(RrUiRow24, byte16, 0x16);
RR_ASSERT_OFFSET(RrUiRow24, byte17, 0x17);
RR_ASSERT_OFFSET(RrUiRow24, half20, 0x20);

/* 0x20-byte UI particle record (D_800F2AE4, initialized by func_80053EC4). */
typedef struct {
    s32 active00, state04, timer08;
    s16 x0C, y0E;
    u8 r10, g11, b12, pad13;
    s16 spanX14, speed16, rot18, scale1A, velX1C, velY1E;
} RrUiParticle20;

RR_ASSERT_SIZE(RrUiParticle20, 0x20);
RR_ASSERT_OFFSET(RrUiParticle20, x0C, 0xC);
RR_ASSERT_OFFSET(RrUiParticle20, r10, 0x10);
RR_ASSERT_OFFSET(RrUiParticle20, spanX14, 0x14);
RR_ASSERT_OFFSET(RrUiParticle20, scale1A, 0x1A);

#endif
