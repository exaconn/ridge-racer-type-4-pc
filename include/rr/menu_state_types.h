#ifndef RR_MENU_STATE_TYPES_H
#define RR_MENU_STATE_TYPES_H

#include "rr/menu_choice_types.h"

/* Persistent menu settings and their four byte-sized selection records. */
typedef struct {
    u8 pad00[8];
    s8 retries08[8];
    u8 reserved10[7];
    s8 phase17;
    u8 pad18[2];
    s16 mode1A;
    RrMenuChoice4 copies[4];
} RrMenuSettings2C;

typedef struct {
    u8 flags00, pad01;
    s8 control02;
    u8 pad03[4], glyph07;
    s16 x08, y0A, w0C, h0E;
    u8 pad10[8];
    s32 phase18;
} RrMenuGlyphRow1C;

#endif
