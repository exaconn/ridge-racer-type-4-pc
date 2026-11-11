#include "rr/badge_rows.h"

void *EmitBadgePatternSprites(void *cursor, s32 style, s32 index) {
    void *result = cursor;
    s32 i = 0;
    s32 xshift;
    s32 glyph;
    const s8 *q;
    BadgeRow1C *p;
    UiUvRect *uv;
    do {
        q = &BadgeStripPatterns[i + style * 8];
        xshift = q[4];
        glyph = q[0];
        p = &BadgeRows[index + i];
        result = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.tags[0],result,p->x08,p->y0A+p->extent0E-2,248,2,8,8,8);
        if(i<4) {
            uv = &BadgeGlyphUvTable[glyph];
            p = &BadgeRows[index + i];
            result = AddSpriteWithDrawMode(&CurrentDrawBuf->buffer.ordering70.tags[0],result,p->x08+xshift,p->y06-2,uv->w,uv->h,uv->u,uv->v,74,0x2E28,1,128);
        }
        p = &BadgeRows[index + i];
        i++;
        result = AddDrawAreaPrim(&CurrentDrawBuf->buffer.ordering70.tags[0],result,p->x14,p->y16,p->w18,p->h1A);
    }while(i<5);
    return result;
}

/* Initial values of the globals this module owns. */
UiUvRect BadgeGlyphUvTable[37] = {{.v = 0x20, .w = 0x18, .h = 0x18}, {.u = 0x18, .v = 0x20, .w = 0x1A, .h = 0x18}, {.u = 0x34, .v = 0x20, .w = 0x20, .h = 0x18}, {.u = 0x54, .v = 0x20, .w = 0x34, .h = 0x18}, {.v = 0x68, .w = 0x14, .h = 0x10}, {.v = 0x68, .w = 0x14, .h = 0x10}, {.u = 0x34, .v = 0x68, .w = 0x20, .h = 0x10}, {.v = 0x78, .w = 0x30, .h = 0x10}, {.u = 0x30, .v = 0x78, .w = 0x28, .h = 0x10}, {.u = 0x58, .v = 0x78, .w = 0x2C, .h = 0x10}, {.u = 0x58, .v = 0x68, .w = 0x38, .h = 0x10}, {.v = 0x88, .w = 0x3C, .h = 0x10}, {.u = 0x3C, .v = 0x88, .w = 0x34, .h = 0x10}, {.u = 0x70, .v = 0x88, .w = 0x48, .h = 0x10}, {.v = 0x98, .w = 0x30, .h = 0x10}, {.u = 0x34, .v = 0x98, .w = 0x3C, .h = 0x10}, {.u = 0x74, .v = 0x98, .w = 0x42, .h = 0x10}, {.v = 0xA8, .w = 0x40, .h = 0x10}, {.v = 0x38, .w = 0x47, .h = 0x10}, {.v = 0x48, .w = 0x3F, .h = 0x10}, {.u = 0x48, .v = 0x38, .w = 4, .h = 0x10}, {.u = 0x2C, .v = 0x58, .w = 0x15, .h = 0x10}, {.u = 0x1C, .v = 0x58, .w = 0xD, .h = 0x10}, {.u = 0xC, .v = 0x58, .w = 0xF, .h = 0x10}, {.v = 0x58, .w = 0xC, .h = 0x10}, {.u = 0x44, .v = 0xA8, .w = 0x28, .h = 0x10}, {.u = 0x6C, .v = 0xA8, .w = 0x20, .h = 0x10}, {.u = 0x8C, .v = 0xA8, .w = 0x20, .h = 0x10}, {.u = 0xB0, .v = 0xA8, .w = 0x1C, .h = 0x10}, {.u = 0x64, .v = 0x3C, .w = 8, .h = 0x20}, {.u = 0x6C, .v = 0x3C, .w = 4, .h = 0x20}, {.u = 0x6C, .v = 0x3C, .w = 0x10, .h = 0x20}, {.u = 0x14, .v = 0x68, .w = 8, .h = 0x10}, {.u = 0x1C, .v = 0x68, .w = 8, .h = 0x10}, {.u = 0x24, .v = 0x68, .w = 8, .h = 0x10}, {.u = 0x2C, .v = 0x68, .w = 8, .h = 0x10}, {.u = 0xBC, .v = 0x88, .w = 0x30, .h = 0x10}};
s8 BadgeStripPatterns[152] = {0, 2, -1, -1, 104, 0, -1, -1, 1, 2, -1, -1, 100, 2, -1, -1, 3, -1, -1, -1, 104, -1, -1, -1, 4, 6, 11, 7, 44, 8, 0, 2, 4, 6, 12, 8, 54, 8, 0, 2, 4, 6, 13, 9, 40, 8, 0, 2, 4, 6, 36, 7, 50, 8, 0, 2, 4, 6, 14, 7, 48, 8, 0, 2, 4, 6, 15, 9, 44, 8, 0, 2, 4, 6, 16, 8, 46, 8, 0, 2, 4, 6, 17, 10, 38, 8, 0, 0, 18, 20, 22, 21, 68, 2, 2, 2, 18, 20, 23, 21, 68, 2, 2, 2, 19, 20, 24, 21, 72, 2, 2, 2, 29, 29, 29, 29, 4, 20, 36, 20, 29, 29, 29, 29, 4, 20, 16, 20, 30, 31, 30, 31, 20, 24, 28, 24, 30, 30, 30, -1, 20, 22, 22, 24, 25, 26, 27, 28, 104, 8, 6, 32};
