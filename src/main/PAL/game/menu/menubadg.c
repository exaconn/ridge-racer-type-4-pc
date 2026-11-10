#include "rr/badge_rows.h"

void *EmitBadgePatternSprites(void *cursor, s32 style, s32 index) {
    /* ASM_BARRIER: stage each returned cursor in A3 before outgoing arguments. */
    register void *result asm("$7") = cursor;
    /* ASM_BARRIER: preserve the unscaled style index in S7 across helpers. */
    register s32 selected asm("$23") = style;
    s32 i = 0;
    const s8 *patterns = BadgeStripPatterns;
    u8 *context_page = (u8 *)0x800B0000U;
    u8 *row_page = (u8 *)0x800F0000U;
    s32 offset = index*28;
    s32 xshift;
    s32 glyph;
    const s8 *q;
    /* ASM_BARRIER: use V1 for row geometry until the final extent load. */
    register BadgeRow1C *p asm("$3");
    BadgeRow1C *last;
    /* ASM_BARRIER: capture the helper cursor in A1 before preparing coordinates. */
    register void *outgoing asm("$5");
    /* ASM_BARRIER: stage the atlas base in V0 before scaling the glyph. */
    register UiUvRect *atlas asm("$2");
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
        /* ASM_BARRIER: end the returned V0 lifetime before the loop comparison. */
        __asm__("" : "+r"(result));
    }while(i<5);
    return result;
}
