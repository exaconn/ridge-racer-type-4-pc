#include "rr/badge_rows.h"

void *EmitBadgePatternSprites(void *cursor, s32 style, s32 index) {
    void *result = cursor;
    s32 selected = style;
    s32 i = 0;
    const s8 *patterns = BadgeStripPatterns;
    u8 *context_page = (u8 *)0x800B0000U;
    u8 *row_page = (u8 *)0x800F0000U;
    s32 offset = index*28;
    s32 xshift;
    s32 glyph;
    const s8 *q;
    BadgeRow1C *p;
    BadgeRow1C *last;
    void *outgoing;
    UiUvRect *atlas;
    UiUvRect *uv;
    do {
        q = (const s8 *)((i+selected*8)+(unsigned long)patterns);
        xshift = q[4];
        glyph = q[0];
        p = (BadgeRow1C *)(offset+(unsigned long)(*(BadgeRow1C **)(row_page+RR_FIELD_OFFSET(BadgeRowPointerPage, rows3AFC))));
        result = AddTilePrim(&(((FrameStorageSlot *)(context_page-0x33EC))->currentDrawBuf)->buffer.ordering70.words[0],result,p->x08,p->y0A+p->extent0E-2,248,2,8,8,8);
        if(i<4) {
            outgoing = result;
            atlas = BadgeGlyphUvTable;
            uv = atlas+glyph;
            p = (BadgeRow1C *)(offset+(unsigned long)(*(BadgeRow1C **)(row_page+RR_FIELD_OFFSET(BadgeRowPointerPage, rows3AFC))));
            result = AddSpriteWithDrawMode(&(((FrameStorageSlot *)(context_page-0x33EC))->currentDrawBuf)->buffer.ordering70.words[0],outgoing,p->x08+xshift,p->y06-2,uv->w,uv->h,uv->u,uv->v,74,0x2E28,1,128);
        }
        outgoing = result;
        i++;
        last = (BadgeRow1C *)(offset+(unsigned long)(*(BadgeRow1C **)(row_page+RR_FIELD_OFFSET(BadgeRowPointerPage, rows3AFC))));
        result = AddDrawAreaPrim(&(((FrameStorageSlot *)(context_page-0x33EC))->currentDrawBuf)->buffer.ordering70.words[0],outgoing,last->x14,last->y16,last->w18,last->h1A);
        offset+=28;
    }while(i<5);
    return result;
}
